struct order{
    int to_floor;
    enum source{outside_elevator, inside_elevator} src;
    enum direction{up, down, irrelevant} dir;
};
