-module(elog_backend).

-compile({no_auto_import,[error/2]}).

-export([start/1, log/2, trace/1, trace/2, debug/1, debug/2, info/1, info/2,
        warn/1, warn/2, error/1, error/2, fatal/1, fatal/2]).

-record(state, {log_level :: integer()
               }).

start(_Config) ->
    LogLevel = application:get_env(elog, log_level, 0),
    {ok, #state { log_level = LogLevel
    }}.

log(Format, Data) ->
    elog:put(lists:flatten(io_lib:format(Format, Data))).


trace(Format) ->
    trace(Format, []).
trace(Format, Data) ->
    LogLevel = application:get_env(elog, log_level, 4),
    case LogLevel > 0 of
        true ->
            ok;
        false ->
            elog:level_put(0, lists:flatten(io_lib:format(Format, Data)))
    end.

debug(Format) ->
    debug(Format, []).
debug(Format, Data) ->
    LogLevel = application:get_env(elog, log_level, 4),
    case LogLevel > 2 of
        true ->
            ok;
        false ->
            elog:level_put(2, lists:flatten(io_lib:format(Format, Data)))
    end.

info(Format) ->
    info(Format, []).
info(Format, Data) ->
    LogLevel = application:get_env(elog, log_level, 4),
    case LogLevel > 4 of
        true ->
            ok;
        false ->
            elog:level_put(4, lists:flatten(io_lib:format(Format, Data)))
    end.

warn(Format) ->
    warn(Format, []).
warn(Format, Data) ->
    LogLevel = application:get_env(elog, log_level, 4),
    case LogLevel > 8 of
        true ->
            ok;
        false ->
            elog:level_put(8, lists:flatten(io_lib:format(Format, Data)))
    end.

error(Format) ->
    error(Format, []).
error(Format, Data) ->
    LogLevel = application:get_env(elog, log_level, 4),
    case LogLevel > 16 of
        true ->
            ok;
        false ->
            elog:level_put(16, lists:flatten(io_lib:format(Format, Data)))
    end.

fatal(Format) ->
    fatal(Format, []).
fatal(Format, Data) ->
    LogLevel = application:get_env(elog, log_level, 4),
    case LogLevel > 32 of
        true ->
            ok;
        false ->
            elog:level_put(32, lists:flatten(io_lib:format(Format, Data)))
    end.
