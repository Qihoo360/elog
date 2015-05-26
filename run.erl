#!/usr/bin/env escript
%% -*- erlang -*-
%%! -smp enable -pa ./ebin/ -name chenzongzhi -setcookie chenzongzhi


timestamp() ->
    {M, S, _} = os:timestamp(),
    M * 1000000 + S.

tamp_micro() ->
    {Mega, Sec, Micro} = erlang:now(),
    %integer_to_list((Mega * 1000000 + Sec) * 1000000 + Micro).
    (Mega * 1000000 + Sec) * 1000000 + Micro.

run_put(N) ->
    case N of
        0 ->
            ok;
        _Other ->
            lager:info("heihei"),
            lager:notice("heihei"),
            lager:warning("heihei"),
            lager:error("heihei"),
            run_put(N - 1)
    end.

elog_put(N) ->
    case N of
        0 ->
            ok;
        _Other ->
            elog_backend:trace("heihei", []),
            elog_backend:info("heihei", []),
            elog_backend:warn("heihei", []),
            elog_backend:error("heihei", []),
            elog_put(N - 1)
    end.

gen_str(N, Str) ->
    case N of
        0 ->
            Str;
        _Other ->
            gen_str(N - 1, Str ++ "cccccccccc")
    end.



main(_) ->
    io:format("line number ~p~n", [?LINE]),
    {ok, D} = file:open("/tmp/erlang", [append]),
    % io:format("elog:put result ~p~n", [elog_backend:warn("put ~s ~B", ["heihei", 40])]),
    % io:format("elog:put result ~p~n", [elog_backend:error("heihei", [])]),
    S = gen_str(9000, ""),
    S1 = S ++ "end",
    elog_backend:error(S1, []),
    % [spawn(fun() -> elog_put(1000) end) || X_<-lists:seq(0, 999)],
    % E = tamp_micro(),
    % io:format(D, "~p elog used time ~p~n", [calendar:local_time(), E - B]),


    % % B1 = tamp_micro(),
    % % % run_put(1000000),
    % % [spawn(fun() -> run_put(1000) end) || X<-lists:seq(0, 1000)],
    % % E1 = tamp_micro(),
    % % io:format(D, "~p lager used time ~p~n", [calendar:local_time(), E1 - B1]),
    timer:sleep(1000),
    io:format("run end ~n", []).
