// lab3-ThreadPool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Matrix.h"
#include <string>
#include <chrono>
#include "lab3-ThreadPool.h"
#include <algorithm>
#include "ThreadPool.h"

void mult(Matrix a, Matrix b, Matrix* res)
{
	ThreadPool pool(4);
	std::vector<std::future<int>> f;

	for (int i = 0; i < a.getRowsNumber(); i++)
		for (int j = 0; j < a.getColsNumber(); j++)
		{
			f.push_back(pool.enqueue([](int line, int column, Matrix a, Matrix b, Matrix* res) {
				int mul = 0;
				for (int k = 0; k < a.getRowsNumber(); k++)
					mul += a.get(line, k) * b.get(k, column);
				res->set(line, column, mul);
				return line;
			}, i, j, a, b, res));
		}
}


void add(Matrix a, Matrix b, Matrix* res)
{
	ThreadPool pool(4);
	std::vector<std::future<int>> f;

	for (int i = 0; i < a.getRowsNumber(); i++)
	{
		f.push_back(pool.enqueue([](int line, Matrix a, Matrix b, Matrix* res) {
			for (int j = 0; j < a.getColsNumber(); j++)
				res->set(line, j, (a.get(line, j) + b.get(line, j)));
			return line;
		}, i, a, b, res));
	}
}


void mult_async(Matrix a, Matrix b, Matrix* res)
{
	std::vector<std::future<int>> f;

	for (int i = 0; i < a.getRowsNumber(); i++)
		for (int j = 0; j < a.getColsNumber(); j++)
		{
			f.push_back(std::async([](int line, int column, Matrix a, Matrix b, Matrix* res) {
				int mul = 0;
				for (int k = 0; k < a.getRowsNumber(); k++)
					mul += a.get(line, k) * b.get(k, column);
				res->set(line, column, mul);
				return line;
			}, i, j, a, b, res));
		}
}


void add_async(Matrix a, Matrix b, Matrix* res)
{
	std::vector<std::future<int>> f;

	for (int i = 0; i < a.getRowsNumber(); i++)
		f.push_back(std::async([](int line, Matrix a, Matrix b, Matrix* res) {
			for (int j = 0; j < a.getColsNumber(); j++)
				res->set(line, j, (a.get(line, j) + b.get(line, j)));
			return line;
		}, i, a, b, res));
}

int main() {

	Matrix a = Matrix(50, 50);
	Matrix b = Matrix(50, 50);
	Matrix res = Matrix(50, 50);

	std::cout << "Matrix a: \n";
	//std::cout << a.printMatrix();

	std::cout << "\nMatrix b: \n";
	//std::cout << b.printMatrix();

	auto start = std::chrono::high_resolution_clock::now();


	std::cout << "\nMatrix sum: \n";
	add(a, b, &res);
	//std::cout << res.printMatrix();
	

	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nAddition " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() % 1000 << "ms\n\n";


	start = std::chrono::high_resolution_clock::now();


	std::cout << "Matrix mult: \n";
	mult(a, b, &res);
	//std::cout << res.printMatrix();


	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nMultiplication " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() % 1000 << "ms\n";




	start = std::chrono::high_resolution_clock::now();


	std::cout << "Matrix sum: \n";
	add(a, b, &res);
	//std::cout << res.printMatrix();


	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nAsync Addition " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() % 1000 << "ms\n\n";


	start = std::chrono::high_resolution_clock::now();


	std::cout << "Matrix mult: \n";
	mult(a, b, &res);
	//std::cout << res.printMatrix();


	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nAsync Multiplication " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() % 1000 << "ms\n";
}


