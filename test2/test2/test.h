# ifndef TEST_H
#define TEST_H
#include<windows.h>
#include<stdio.h>
#include<tchar.h>
#include<memory>
#include<random>
#include<iostream>
#include<string>
#include<ctime>

using namespace std;
string letter = "abcdefghijklmnopqrstuvwxyzABCDEFGHIZKLMNOPQRSTUVWXYZ";
default_random_engine engine;
uniform_int_distribution<int>uid(5,20);
uniform_int_distribution<int>uid1(100, 200);
uniform_int_distribution<int>uid2(0, 51);


#endif