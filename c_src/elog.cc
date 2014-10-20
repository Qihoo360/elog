#include <iostream>
#include <string>
#include "stdio.h"

#include "erl_nif.h"

#include "filehandler.h"
#include "levelhandler.h"

// Atoms (initialized in on_load)
namespace log_item 
{
ERL_NIF_TERM ATOM_OK;
ERL_NIF_TERM ATOM_ERROR;
}

ErlNifResourceType* mario_type;

ErlNifResourceType* handle_type;

void
free_mario(ErlNifEnv* env, void* obj)
{
    mario::Mario *m;
    m = (mario::Mario *)obj;
    delete(m);
    return ;
}

void
free_handle(ErlNifEnv* env, void* obj)
{
    FileHandler *fh;
    fh = (FileHandler *)obj;
    delete(fh);
    return ;
}

static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
    ErlNifResourceFlags flags = (ErlNifResourceFlags)(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER);
    const char* mod = "elog";

    const char* mario_name = "mario";
    mario_type = enif_open_resource_type(env, mod, mario_name, free_mario, flags, NULL);
    if (mario_type == NULL) {
        return -1;
    }

    const char* handle_name = "filehandle";
    handle_type = enif_open_resource_type(env, mod, handle_name, free_handle, flags, NULL);
    if (handle_type == NULL) {
        return -1;
    }

    if (!enif_is_list(env, load_info)) {
        return -1;
    }

    // {filename, "log/czz.log"}  % add the filename
    // get filename from the load_nif
    // enif_get_list_cell set load_info_head to the load_info head and load_info to the load_info end
    ERL_NIF_TERM load_info_head;
    char filename[1024];
    while (0 != enif_get_list_cell(env, load_info, &load_info_head, &load_info)) {
        int arity = 0;
        ERL_NIF_TERM *tuple_data;
        // get this tuple {filename, "log/czz.log"}
        if (enif_get_tuple(env, load_info_head, &arity, const_cast<const ERL_NIF_TERM **>(&tuple_data))) {
            unsigned int atom_len;
            if (0 == enif_get_atom_length(env, tuple_data[0], &atom_len, ERL_NIF_LATIN1)) {
                continue;
            }
            const unsigned int atom_max = 128;
            char atom[atom_max];
            // get the atom filename from tuple {filename, "log/czz.log"}
            if ((atom_len + 1) != static_cast<unsigned int>(enif_get_atom(env, tuple_data[0], atom, atom_max, ERL_NIF_LATIN1))) {
                continue;
            }
            if (0 != strncmp(atom, "filename", atom_max)) {
                continue;
            }
            if (0 == enif_get_string(env, tuple_data[1], filename, sizeof(filename), ERL_NIF_LATIN1)) {
                return -1;
            }
        }
    }

    mario::Mario *m;

    // FileHandler *fh;
    // fh = new FileHandler(1024 * 1024 * 20, filename, "./log");

    LevelHandler *lh;
    lh = new LevelHandler(1024 * 1024 * 20, filename, "./log");
    m = new mario::Mario(8, lh);
    *priv = m;

#define ATOM(Id, Value) { Id = enif_make_atom(env, Value); }
    ATOM(log_item::ATOM_OK, "ok");
    ATOM(log_item::ATOM_ERROR, "error");
    return 0;
}

static int
upgrade(ErlNifEnv* env, void** priv, void** old_priv, ERL_NIF_TERM load_info)
{
    return 0;
}

static void
unload(ErlNifEnv* env, void* priv)
{

    return;
}

static ERL_NIF_TERM
put(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{

    char item[1024];

    mario::Mario& m = *static_cast<mario::Mario *>(enif_priv_data(env));

    if (!enif_get_string(env, argv[0], item, sizeof(item), ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    mario::Status s;
    s = m.Put(item);
    if (s.ok()) {
        return log_item::ATOM_OK;
    } else {
        return log_item::ATOM_ERROR;
    }
}

static ERL_NIF_TERM
level_put(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    char item[1024];
    int level;

    mario::Mario& m = *static_cast<mario::Mario *>(enif_priv_data(env));

    if (!enif_get_int(env, argv[0], &level)) {
        return enif_make_badarg(env);
    }

    memcpy(item, &level, sizeof(int));
    if (!enif_get_string(env, argv[1], item + sizeof(int), sizeof(item), ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    mario::Status s;
    s = m.Put(std::string(item, sizeof(item)));
    if (s.ok()) {
        return log_item::ATOM_OK;
    } else {
        return log_item::ATOM_ERROR;
    }
}

static ErlNifFunc nif_funcs[] = {
    {"put", 1, put},
    {"level_put", 2, level_put}
};

ERL_NIF_INIT(elog, nif_funcs, &load, NULL, &upgrade, &unload);
