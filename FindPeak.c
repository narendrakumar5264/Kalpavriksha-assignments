#include <stdio.h>

int findPeakElement(int nums[], int length) {
    int left = 0, right = length - 1;
    while (left < right) {
        int middle = left + (right - left) / 2;
        if (nums[middle] > nums[middle + 1])
            right = middle;
        else
            left = middle + 1;
    }
    return left;
}

int main() {
    int nums[] = {1,2,1,3,5,6,4};
    int length = sizeof(nums) / sizeof(nums[0]);
    int result = findPeakElement(nums, length);
    printf("%d", result);
    return 0;
}
