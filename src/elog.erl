-module(elog).

-export([open/1, put/1, level_put/2]).
-on_load(init/0).

-define(APPNAME, elog).
-define(LIBNAME, elog).

% -opaque db_ref() :: binary().

% -spec open(string()) -> {ok} | {error}.
open(_Name) ->
    not_loaded(?LINE).

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
    erlang:load_nif(SoName, [
            {filename, "log/bada."}  % add the file name
        ]).

not_loaded(Line) ->
    exit({not_loaded, [{module, ?MODULE}, {line, Line}]}).
