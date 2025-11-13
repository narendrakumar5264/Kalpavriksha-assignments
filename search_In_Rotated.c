#include <stdio.h>

int searchInRotatedArray(int nums[], int length, int target) {
    int left = 0, right = length - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        if (nums[middle] == target)
            return middle;
        if (nums[left] <= nums[middle]) {
            if (target >= nums[left] && target < nums[middle])
                right = middle - 1;
            else
                left = middle + 1;
        } else {
            if (target > nums[middle] && target <= nums[right])
                left = middle + 1;
            else
                right = middle - 1;
        }
    }
    return -1;
}

int main() {
    int nums[] = {4,5,6,7,0,1,2};
    int target = 0;
    int length = sizeof(nums) / sizeof(nums[0]);
    int result = searchInRotatedArray(nums, length, target);
    printf("%d", result);
    return 0;
}
