pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
void *downtime() {
    int count = 0;
    while (count < 4) { //Program ends after 3 downtimes. Needs to be checked for < 3 & < 4
        sleep(2);
        balanceTree(root);
        count++;
    }
    return NULL;
}

void *ServeClient(char *client) {
    return NULL;
}
