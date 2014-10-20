-module(elog).

-export([put/1, level_put/2]).
-on_load(init/0).

-define(APPNAME, elog).
-define(LIBNAME, elog).


-spec put(string()) -> {ok} | {error}.
put(_Arg) ->
    not_loaded(?LINE).

level_put(_Level, _Arg) ->
    not_loaded(?LINE).

-spec init() -> ok | {error, any()}.
init() ->
    SoName = case code:priv_dir(?APPNAME) of
        {error, bad_name} ->
            case filelib:is_dir(filename:join(["..", priv])) of
                true ->
                    filename:join(["..", priv, ?LIBNAME]);
                _ ->
                    filename:join([priv, ?LIBNAME])
            end;
        Dir ->
            filename:join(Dir, ?LIBNAME)
    end,
    Pre = application:get_env(elog, logname, "elog"),
    erlang:load_nif(SoName, [
            {filename, "log/" ++ Pre ++ "."}  % add the file name
        ]).

not_loaded(Line) ->
    exit({not_loaded, [{module, ?MODULE}, {line, Line}]}).
