-module(elog_tests).

-export([basic_test/0]).

-include_lib("eunit/include/eunit.hrl").

basic_test() ->
    elog:hello().
