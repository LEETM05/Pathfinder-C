#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

#define MAX_CITY 56
#define TRUE 1
#define FALSE 0
#define SIZE 20
#define INF 1000000

typedef struct {
	char name[SIZE];
	int time[MAX_CITY];
	int cost[MAX_CITY];
	int row;
	int col;
} MAP;

bool v[MAX_CITY];
int Cost[MAX_CITY];
int _time[MAX_CITY];
MAP data[MAX_CITY + 1];
int transit[MAX_CITY];
int real_virtual_price[MAX_CITY + 1];

void initialize(MAP data[]);
void separate_field(char* s, char* token, MAP* data);
int name_matching(char* str);
int getSmallIndex(int choose);
void dijkstara(int start, int choose, MAP G[]);
void option(int start, int end);
void path_node(int start, int end);
void print_map(MAP data[]);
void gotoXY(int _x, int _y);
void Expect_Time(int end);