
int foo(int x) {
  switch (x) {
  case 1:
  case 2:
    x = x + 1;
  case 3:
    x = x + 2;
  default:
    return 2;
  }
}

