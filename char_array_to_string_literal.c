#include <stdio.h>
#include <string.h>

int main() {
  int i;
  char arr[] = {
    0x38, 0x3d, 0x46, 0x49, 0x58, 0x2e, 0x34, 0x2e,
    0x32, 0x01, 0x39, 0x3d, 0x38, 0x37, 0x01, 0x33,
    0x35, 0x3d, 0x57, 0x01, 0x33, 0x34, 0x3d, 0x32,
    0x01, 0x34, 0x39, 0x3d, 0x41, 0x43, 0x43, 0x45,
    0x50, 0x54, 0x4f, 0x52, 0x01, 0x35, 0x32, 0x3d,
    0x32, 0x30, 0x31, 0x37, 0x30, 0x32, 0x31, 0x33,
    0x2d, 0x32, 0x30, 0x3a, 0x35, 0x36, 0x3a, 0x30,
    0x37, 0x2e, 0x38, 0x36, 0x38, 0x01, 0x35, 0x36,
    0x3d, 0x49, 0x4e, 0x49, 0x54, 0x49, 0x41, 0x54,
    0x4f, 0x52, 0x01, 0x32, 0x32, 0x3d, 0x38, 0x01,
    0x34, 0x38, 0x3d, 0x01, 0x35, 0x35, 0x3d, 0x01,
    0x32, 0x30, 0x37, 0x3d, 0x42, 0x56, 0x4d, 0x46,
    0x01, 0x32, 0x36, 0x32, 0x3d, 0x01, 0x31, 0x30,
    0x3d, 0x30, 0x32, 0x31, 0x01
  };

  printf("\"");
  for (i = 0; i < strlen(arr); i++) {
    if (arr[i] >= 32) {
      printf("%c", arr[i]);
    } else {
      printf("\"\"\\x%x\"\"", arr[i]);
    }
  }
  printf("\"\n");

  return 0;
}
