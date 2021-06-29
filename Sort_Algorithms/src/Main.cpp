#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <algorithm>

#define VAR_TO_STR(var) #var

/* Size of the list to sort. */
constexpr std::size_t LIST_SIZE = 10000;

using namespace std;

/* Stores each sorting function with it's execution duration. */
struct SortFunc
{
	std::string name;
	void(*func)(std::vector<int>&);
	std::vector<std::chrono::duration<double, std::milli>> durations;
};

/* Finds the minimum in the list and puts it to the beginning. O(n^2). */
void selectionSort(std::vector<int>& list)
{
	for (int i = 0; i < list.size() - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < list.size(); j++)
		{
			if (list[j] < list[min])
			{
				min = j;
			}
		}
		int temp = list[i];
		list[i] = list[min];
		list[min] = temp;
	}
}

/* Slightly optimized selection sort. No memory allocation in each pass of the outer loop. O(n^2). */
void selectionSortOptimized(std::vector<int>& list)
{
	int minIndex, temp, j;
	for (int i = 0; i < list.size() - 1; i++)
	{
		minIndex = i;
		for (j = i + 1; j < list.size(); j++)
		{
			if (list[j] < list[minIndex])
			{
				minIndex = j;
			}
		}
		temp = list[i];
		list[i] = list[minIndex];
		list[minIndex] = temp;
	}
}

/* Each pass it swaps each element with it's predecessor if the element is smaller.
   Saves the index of the last changed element since all elements above this
   index are already sorted. O(n^2). */
void bubblesort(std::vector<int>& list)
{
	std::size_t n = list.size();
	int newn = 0;
	int i;
	int temp;
	while (n > 1)
	{
		newn = 0;
		for (i = 1; i < n; i++)
		{
			if (list[i - 1] > list[i])
			{
				temp = list[i - 1];
				list[i - 1] = list[i];
				list[i] = temp;
				newn = i;
			}
		}
		n = newn;
	}
}

/* Bubblesort but with std::swap instead of a temporary variable. */
void bubblesortStdSwap(std::vector<int>& list)
{
	std::size_t n = list.size();
	int newn = 0;
	int i;
	int temp;
	while (n > 1)
	{
		newn = 0;
		for (i = 1; i < n; i++)
		{
			if (list[i - 1] > list[i])
			{
				std::swap(list[i - 1], list[i]);
				newn = i;
			}
		}
		n = newn;
	}
}

/* Insertion sort that sorts a sublist in list. */
void insertionSort(std::vector<int>& list, int low, int high)
{
	if (low >= high)
	{
		cout << "InsertionSort low >= high." << endl;
		return;
	}
	else if (high >= list.size())
	{
		cout << "InsertionSort high >= list.size()." << endl;
		return;
	}

	int temp;
	for (int i = low + 1; i <= high; i++)
	{
		for (int j = i; j > low && list[j - 1] > list[j]; j--)
		{
			temp = list[j - 1];
			list[j - 1] = list[j];
			list[j] = temp;
		}
	}
}

/* Each pass it swaps the i-th element with it's predecessor until
   the predecessor is smaller. Starts with i=1 to i = list.size - 1. */
void insertionSort(std::vector<int>& list)
{
	insertionSort(list, 0, list.size() - 1);
}

/* Merges two sorted list into one sorted list. Used by Mergesort. */
std::vector<int> merge(const std::vector<int>& a, const std::vector<int>& b)
{
	std::vector<int> merged;
	int aIndex = 0;
	int bIndex = 0;
	while (aIndex + bIndex < a.size() + b.size())
	{
		if (bIndex >= b.size() || (aIndex < a.size() && a[aIndex] < b[bIndex]))
		{
			merged.push_back(a[aIndex++]);
		}
		else
		{
			merged.push_back(b[bIndex++]);
		}
	}

	return merged;
}

/* Splits the list into two same sized lists, then sorts the splitted lists and
   finally merges them back to a single list. 
   If the list contains only one element the list is sorted. */
void mergesortRecursive(std::vector<int>& list)
{
	if (list.size() == 1)
	{
		return;
	}

	int mid = list.size() / 2;
	std::vector<int> a;
	std::vector<int> b;
	for (int i = 0; i < mid; i++) a.push_back(list[i]);
	for (int i = mid; i < list.size(); i++) b.push_back(list[i]);
	mergesortRecursive(a);
	mergesortRecursive(b);

	list = merge(a, b);
}

/* Mergesort without recursion. */
void mergesortIterative(std::vector<int>& list)
{
	int aIndex, bIndex;
	std::vector<int> temp;
	for (int i = 1; i <= ceil(log2(list.size())); i++)
	{
		for (int j = 0; j < list.size(); j += pow(2, i))
		{
			aIndex = j;
			bIndex = j + 0.5 * pow(2, i);
			while (aIndex <= j + 0.5 * pow(2, i) && bIndex <= j + pow(2, i) && bIndex <= list.size() && !(aIndex == j + 0.5 * pow(2, i) && (bIndex == j + pow(2, i) || bIndex >= list.size())))
			{
				if ((bIndex >= list.size() || list[aIndex] < list[bIndex] || bIndex >= j + pow(2, i)) && aIndex < j + 0.5 * pow(2, i))
				{
					temp.push_back(list[aIndex++]);
				}
				else
				{
					temp.push_back(list[bIndex++]);
				}
			}

			for (int k = 0; k < temp.size(); k++)
			{
				list[k + j] = temp[k];
			}
			temp.clear();
		}
	}
}

/* Sorts the list from index low to high with quicksort. */
void quicksort(std::vector<int>& list, int low, int high)
{
	if (low < high)
	{
		int& pivot = list[high];
		int temp;
		int i = low;
		for (int j = low; j <= high; j++)
		{
			if (list[j] < pivot)
			{
				temp = list[j];
				list[j] = list[i];
				list[i] = temp;
				i++;
			}
		}
		temp = list[high];
		list[high] = list[i];
		list[i] = temp;

		quicksort(list, low, i - 1);
		quicksort(list, i + 1, high);
	}
}

/* Picks the last element as a pivot and puts all smaller elements
   before and all bigger elements after that pivot.
   Then quicksort is called again with the list before and the list
   after the pivot until the list consists of only one element. */
void quicksortRecursive(std::vector<int>& list)
{
	quicksort(list, 0, list.size() - 1);
}

/* Merges two sorted sublists inside a list into one sorted sublist. Used by timsort. */
void merge(std::vector<int>& list, int l, int m, int r)
{
	int len1 = m - l + 1;
	int len2 = r - m;
	std::vector<int> left;
	left.reserve(len1);
	left.insert(left.end(), list.begin() + l, list.begin() + m + 1);
	std::vector<int> right;
	right.reserve(len2);
	right.insert(right.end(), list.begin() + m + 1, list.begin() + r + 1);

	int i = 0;
	int j = 0;
	int k = l;

	while (i < len1 && j < len2)
	{
		if (left[i] <= right[j])
		{
			list[k] = left[i];
			i++;
		}
		else
		{
			list[k] = right[j];
			j++;
		}
		k++;
	}

	while (i < len1)
	{
		list[k] = left[i];
		k++;
		i++;
	}

	while (j < len2)
	{
		list[k] = right[j];
		k++;
		j++;
	}
}

/* First sorts sublists of size runSize with insertion sort.
   Then merges these sorted lists like mergesort. */
void timsort(std::vector<int>& list)
{
	const int runSize = 32;

	for (int i = 0; i < list.size() - 1; i += runSize)
		insertionSort(list, i, min((i + runSize - 1), (int)(list.size() - 1)));

	for (int size = runSize; size < list.size(); size *= 2)
	{
		for (int left = 0; left < list.size(); left += 2 * size)
		{
			int mid = left + size - 1;
			int right = min((left + 2 * size - 1), (int)(list.size() - 1));

			if (mid < right)
				merge(list, left, mid, right);
		}
	}
}

/* Wrapper for std::sort. */
void stdSort(std::vector<int>& list)
{
	std::sort(list.begin(), list.end());
}

/* Prints a list to the console. */
void printList(std::vector<int>& list)
{
	for (int i = 0; i < list.size(); i++)
	{
		std::cout << list[i] << ((i + 1) % 40 == 0 ? '\n' : ' ');
	}
	std::cout << std::endl;
}

/* Checks if the given list is sorted low to high. */
bool checkList(std::vector<int>& list)
{
	for (int i = 1; i < list.size(); i++)
	{
		if (list[i - 1] > list[i])
			return false;
	}

	return true;
}

/* Measures the execution time of the given sorting function and list. */
std::chrono::duration<double, milli> measureFunction(SortFunc& sortFunc, std::vector<int>& list)
{
	using namespace std::chrono;
	auto start = high_resolution_clock::now();
	sortFunc.func(list);
	auto stop = high_resolution_clock::now();
	duration<double, std::milli> duration = stop - start;

	return duration;
}

int main()
{
	using namespace std::chrono;

	srand(0);

	std::vector<int> a{ 0, 1, 2, 3, 4, 5 };
	cout << a.size() << endl;
	std::vector<int> b{ 11, 11, 11 };
	std::vector<int> c;
	c.reserve(a.size());
	c.insert(c.end(), a.begin() + 1, a.begin() + 3);
	printList(a);
	printList(c);
	cout << "--------" << endl;
	timsort(a);
	printList(a);

	// Create structs with sort functions
	std::vector<SortFunc> sortFuncs
	{
		//{ VAR_TO_STR(selectionSort), selectionSort },
		//{ VAR_TO_STR(selectionSortOptimized), selectionSortOptimized },
		//{ VAR_TO_STR(bubblesort), bubblesort },
		//{ VAR_TO_STR(bubblesortSwap), bubblesortStdSwap },
		{ VAR_TO_STR(insertionSort), insertionSort },
		{ VAR_TO_STR(mergesortRecursive), mergesortRecursive },
		{ VAR_TO_STR(mergesortIterative), mergesortIterative },
		{ VAR_TO_STR(quicksortRecursive), quicksortRecursive },
		{ VAR_TO_STR(timsort), timsort },
		{ VAR_TO_STR(stdSort), stdSort },
	};

	std::vector<int> list, copyList;

	// Measure time of each sort function
	for (int i = 0; i < 1; i++)
	{
		list.clear();
		list.reserve(LIST_SIZE);
		for (int i = 0; i < LIST_SIZE; i++)
		{
			list.push_back(rand() % 1000);
		}
		srand((unsigned)time(NULL));

		for (SortFunc& sortFunc : sortFuncs)
		{
			copyList = list;
			sortFunc.durations.push_back(measureFunction(sortFunc, list));
			if (!checkList(list))
				cout << "Not in order: " << VAR_TO_STR(sortFunc.name) << "\n";
			list = copyList;
		}
	}

	// Calculate mean duration for each function and print results
	for (SortFunc& sortFunc : sortFuncs)
	{
		duration<double, std::milli> mean(0.0);
		for (duration<double, std::milli>& dur : sortFunc.durations)
		{
			mean += dur;
		}
		mean /= sortFunc.durations.size();

		std::cout << sortFunc.name << " " << std::string(30 - sortFunc.name.size(), '.') << " " << mean.count() << " ms" << std::endl;
	}

	//std::cin.get();
	return 0;
}