/*
 * Nonogram/paint-by-numbers solver in SWI-Prolog. Uses CLP(FD),
 * in particular the automaton/3 (finite-state/RE) constraint.
 * Copyright (c) 2011 Lars Buitinck.
 * Do with this code as you like, but don't remove the copyright notice.
 */

:- use_module(library(clpfd)).

nono(RowSpec, ColSpec, Grid) :-
    rows(RowSpec, Grid),
    transpose(Grid, GridT),
    rows(ColSpec, GridT).

rows([], []).
rows([C|Cs], [R|Rs]) :-
    row(C, R),
    rows(Cs, Rs).

row(Ks, Row) :-
    sum(Ks,  #=, Ones),
    sum(Row, #=, Ones),
    arcs(Ks, Arcs, start, Final),
    append(Row, [0], RowZ),
    automaton(RowZ, [source(start), sink(Final)], [arc(start,0,start) | Arcs]).

% Make list of transition arcs for finite-state constraint.
arcs([], [], Final, Final).
arcs([K|Ks], Arcs, CurState, Final) :-
    gensym(state, NextState),
    (K == 0 ->
        Arcs = [arc(CurState,0,CurState), arc(CurState,0,NextState) | Rest],
        arcs(Ks, Rest, NextState, Final)
    ;
        Arcs = [arc(CurState,1,NextState) | Rest],
        K1 #= K-1,
        arcs([K1|Ks], Rest, NextState, Final)
    ).


make_grid(Grid, X, Y, Vars) :-
    length(Grid,X),
    make_rows(Grid, Y, Vars).
make_rows([], _, []).
make_rows([R|Rs], Len, Vars) :-
    length(R, Len),
    make_rows(Rs, Len, Vars0),
    append(R, Vars0, Vars).

print([]).
print([R|Rs]) :-
    print_row(R),
    print(Rs).
print_row([]) :- nl.
print_row([X|R]) :-
    (X == 0 ->
        write(' ')
    ;
        write('x')
    ),
    print_row(R).

example_dog(6, 6,
	[[2], [2,1], [4,1], [4], [1,1], [1,1]],
	[[2], [2], [1,4], [4],[1],[4]] ).

example_bear(10, 10,
	[[3,3], [3,3], [7], [1,1], [1,1,1,2], [1,1], [1,2,1], [2,1], [2,2], [4]],
	[[2,5], [3,1], [3,1], [1,1,1], [1,1], [1,1,1], [3,1,1,1], [4,2], [2,1,1], [4]] ).

example_coala(20, 20,
	[
		[2,2,2],[3,1,7,1],[3,1,9,1],[4,13],[4,1,3,3],[4,2,5],[5,2,5],[4,2,5],
		[4,7],[5,7],[3,9],[2,7,6],[1,5,7],[2,9],[6,2,4],[5,6],[3,8],[3,4], [3],[4]
	],
	[
		[5],[7],[9],[9],[9],[5,2],[1,2,2],[3,2,4],[1,1,2,5],[1,6,2,6],[3,4,3,2,2],
		[4,4,1,2,1],[4,4,2,2,1],[10,3,2],[3,9,3],[13,2],[16],[1,6,8],[1,1,7],[3,5]] ).

example_drakkar(50, 50,
	[
		[1], [1],
		[21], [2,18],
		[1,4,8], [1,3,2,4],
		[1,2,2,5], [2,2,1,4],
		[1,2,2,4], [2,2,1,1],
		[1,2,2,3,1], [2,1,1,2,2],
		[1,2,1,7], [2,1,1,6],
		[1,1,1,3,2], [2,2,1,6],
		[1,1,1,4,2], [1,1,1,1,2],
		[2,1,1,1,2], [1,1,1,1,2],
		[2,2,1,1,3], [2,1,1,1,4],
		[2,1,1,2,5], [2,1,2,1,6],
		[1,2,1,2,3,3], [1,1,12,2,3,2],
		[1,1,4,3,3,1,1], [3,1,3,1,1,3,1,1],
		[3,1,2,1,4,3,1], [1,3,1,1,5,3,1,1],
		[2,8,1,5,3,1,1], [9,6,4,4,1],
		[3,13,4,5], [13,4,5,4],
		[2,2,4,4,5,2], [2,2,4,9,5],
		[3,2,15,4,1], [5,7,2],
		[3,16,2], [3,2,2],
		[5,4,4,4,3,2,7], [5,6,3,3],
		[7,3,8,2,3,3], [14,4,1,5],
		[5,12,5,3], [7,13,3,5],
		[12,4], [4,7,7,3,9],
		[1,3], [13,6,5,16,5]
	],
	[
		[2,1,1], [2,1,1,1],
		[1,1,1,1,1,1,1], [1,1,1,1,1,1,1],
		[2,1,1,1,1,1], [3,1,1,1,1,1,1,1],
		[1,3,1,1,1,2,1,1], [1,3,1,1,1,1,1,1,1],
		[1,3,5,1,1,2,1,1,1], [1,3,9,1,1,2,1,1],
		[1,3,4,3,3,1,2,1,1], [1,4,5,2,2,1,1,1],
		[1,6,3,2,2,1,1,1], [1,13,2,1,1,1],
		[2,3,2,2,1,1,1], [2,4,2,1,1,1,1,1],
		[2,1,3,2,2,1,1,1,1], [3,2,1,1,1,1,1,1,1,1],
		[4,1,3,1,1,1,1,1,1,1], [5,2,2,1,1,2,1,1,1,1],
		[7,1,2,1,1,1,1,3,1], [4,4,7,1,1,1,1,2,1],
		[2,4,2,2,1,1,2,2,1,1], [1,4,1,2,1,1,2,2,1,1],
		[1,6,2,1,1,1,1,1,2,1,1], [1,6,2,1,1,1,1,2,1,1],
		[1,1,1,1,1,1,1,2,1,1], [2,1,1,1,1,1,1,1,1,1],
		[2,1,2,1,2,1,1,1,2,1], [3,1,1,1,2,1,2,1,1,1],
		[4,1,1,1,1,1,1,1,1,1], [2,3,1,1,1,1,1,2,2,1],
		[1,3,1,2,1,1,1,1,1,2,1], [1,4,1,1,1,1,1,1,1,3,1],
		[2,4,2,1,1,1,1,2,2,1,2,1], [1,6,1,2,1,2,2,1,1,3,1],
		[2,4,1,2,1,3,1,1,1], [2,1,1,1,1,1,1,2,1,1],
		[1,2,2,1,1,1,1,1,1,1,1], [2,1,3,2,1,2,1,2,1,1,2,1],
		[3,3,3,3,1,1,2,1,1,1,1], [7,4,3,1,1,1,2,1,1,1],
		[2,5,4,2,1,1,1,1,1,1,1], [1,13,1,2,2,2,1,1,1,1],
		[13,2,1,1,3,1,1,1], [4,1,5,1,1,1,4,1,1,1,1],
		[5,18,1,2,1], [3,3,4,1,1,1,4],
		[5,1,1,1,1,1,2], [1,1,1,1,1,2]] ).
go1 :-
    example_dog(X, Y, Rows, Cols),
    make_grid(Grid, X, Y, Vars),
    nono(Rows, Cols, Grid),
    label(Vars),
    print(Grid).

go2 :-
    example_bear(X, Y, Rows, Cols),
    make_grid(Grid, X, Y, Vars),
    nono(Rows, Cols, Grid),
    label(Vars),
    print(Grid).

go3 :-
    example_coala(X, Y, Rows, Cols),
    make_grid(Grid, X, Y, Vars),
    nono(Rows, Cols, Grid),
    label(Vars),
    print(Grid).

go4 :-
    example_drakkar(X, Y, Rows, Cols),
    make_grid(Grid, X, Y, Vars),
    nono(Rows, Cols, Grid),
    label(Vars),
    print(Grid).

test :-
    time(go1),
    time(go2),
    time(go3),
    time(go4).
