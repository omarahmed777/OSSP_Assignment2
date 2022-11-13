pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void *downtime()
{
    int count = 1; //count = 1 makes the while loop being < 4 more intuitive to read imo
    while (count < 4)
    { //Program ends after 3 downtimes.
        sleep(2);
        balanceTree(root);
        count++;
    }
    return NULL;
}

void *ServeClient(char *client)
{
    FILE *fp;
    char line[20]; //Char array to store each line
    char *clientName; //String to hold client name for server log output
    char *commands; //Kind of useless, just need something to hold the rest of the string for strtok_r()
    if ((fp = fopen(client, "r")) == NULL) //Open file & check opening correctly
    {
        printf("File cannot be opened\n");
        exit(-1);
    }
    while (1) //Loop until EOF
    {
        if(fgets(line, 20, fp) == NULL) break;
        else
        {
            char *foo; //Function name
            char *arg; //If applicable, arg for the function
            foo = strtok_r(line, " ", &arg);
            if (strcmp(foo, line) == 0)
            {
                pthread_rwlock_rdlock(&rwlock);
                if (strcmp(foo, "avgSubtree") == 0)
                {
                    avgSubtree(root);
                    clientName = strtok_r(client, "_", &commands);
                    printf("%savgSubtree", clientName);
                }
                else if(strcmp(foo, "countNodes") == 0)
                {
                    int total = countNodes(root);
                    clientName = strtok_r(client, "_", &commands);
                    printf("%scountNodes = %d", clientName, total);
                }
                pthread_rwlock_unlock(&rwlock);
            }
            else
            {
                arg = strtok_r(NULL, " ", &arg);
                if (strcmp(foo, "addNode") == 0)
                {
                    pthread_rwlock_wrlock(&rwlock);
//                    printf("addNode Lock\n");
                    int real_arg = atoi(arg);
                    root = addNode(root, real_arg);
                    clientName = strtok_r(client, "_", &commands);
                    printf("%sinsertNode %s", clientName, arg);
                    pthread_rwlock_unlock(&rwlock);
                } else if (strcmp(foo, "removeNode") == 0)
                {
                    pthread_rwlock_wrlock(&rwlock);
//                    printf("removeNode Lock\n");
                    int real_arg = atoi(arg);
                    root = removeNode(root, real_arg);
                    clientName = strtok_r(client, "_", &commands);
                    printf("%sdeleteNode %s", clientName, arg);
                    pthread_rwlock_unlock(&rwlock);
                }
            }
//            printf("%s ", foo);
//            printf("%s", arg);
        }
    }
    fclose(fp);
    return NULL;
}
