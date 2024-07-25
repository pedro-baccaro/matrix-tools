struct IntTuple{
    int first;
    int second;
};

struct IntTuple newIntTuple(int first, int second){
    struct IntTuple result;
    result.first = first;
    result.second = second;
    return result;
}

void showIntTuple(struct IntTuple t){
    printf("( %d , %d )\n", t.first, t.second);
    return;
}

int compareIntTuple(struct IntTuple a, struct IntTuple b){
    if (a.first == b.first && a.second == b.second)
        return 1;
    return 0;
}