#pragma once

#ifndef _ALGORITHM
#define _ALGORITHM

#include <vector>
#include <queue>

namespace DX
{
	namespace Algorithm {

		inline void SelectionSort(std::vector<int>& arr)
		{
			for (int i = 0; i < arr.size() - 1; ++i)
			{
				for (int j = i + 1; j < arr.size(); ++j)
				{
					if (arr[i] > arr[j])
					{
						int temp = arr[i];
						arr[i] = arr[j];
						arr[j] = temp;
					}
				}
			}
		}

		inline void BubbleSort(std::vector<int>& arr)
		{
			for (int i = 0; i < arr.size() - 1; ++i)
			{
				for (int j = 0; j < arr.size() - 1; ++j)
				{
					if (arr[j] > arr[j + 1])
					{
						int temp = arr[j];
						arr[j] = arr[j + 1];
						arr[j + 1] = temp;
					}
				}
			}
		}

		inline std::vector<int> MergeRec(const std::vector<int>& arr, int first, int last)
		{
			std::vector<int> mergeV;

			if (first == last)
			{
				mergeV.push_back(arr[first]);
			}
			else
			{
				int mid = first + (last - first) / 2;
				std::vector<int> leftV = MergeRec(arr, first, mid);
				std::vector<int> rightV = MergeRec(arr, mid + 1, last);

				int lIdx = 0;
				int rIdx = 0;
				while (lIdx < leftV.size() || rIdx < rightV.size())
				{
					int curSmallLeft = INT_MAX;
					int curSmallRight = INT_MAX;
					if (lIdx < leftV.size())
					{
						curSmallLeft = leftV[lIdx];
					}
					if (rIdx < rightV.size())
					{
						curSmallRight = rightV[rIdx];
					}

					if (curSmallLeft < curSmallRight)
					{
						mergeV.push_back(curSmallLeft);
						lIdx++;
					}
					else
					{
						mergeV.push_back(curSmallRight);
						rIdx++;
					}
				}
			}

			return mergeV;
		}

		inline void MergeSort(std::vector<int>& arr)
		{
			std::vector<int> sorted = MergeRec(arr, 0, arr.size() - 1);

			for (int i = 0; i < arr.size(); ++i)
			{
				arr[i] = sorted[i];
			}
		}
	}
}

#endif