void throwError(){
    volatile int x = 0;
    printf("%d", 1/x);
    return;
}