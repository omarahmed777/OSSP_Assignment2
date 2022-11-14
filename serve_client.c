pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int count = 1; //count = 1 makes the if statement being < 4 more intuitive to read imo
void *downtime()
{
    if (count < 4)
    {
        sleep(2);
        pthread_rwlock_wrlock(&rwlock);
        root_balanced = balanceTree(root);
        pthread_rwlock_unlock(&rwlock);
        count += 1;
    }
    return NULL;
}

void *ServeClient(char *client)
{
    FILE *fp;
    char line[30]; //Char array to store each line
    //char fileName[30];
    if ((fp = fopen(client, "r")) == NULL) //Open file & check opening correctly
    {
        printf("File cannot be opened\n");
        exit(-1);
    }
    while (1) //Loop until EOF
    {
        if(fgets(line, 30, fp) == NULL) break;
        else
        {
            // Extracting function name from *client
            char *foo; //Function name
            char *arg; //If applicable, arg for the function
            foo = strtok_r(line, " ", &arg);

            // Extracting clientName from *client
            char clientName[10]; //String to hold client name for server log output
            char *c = "_"; //Character to be found
            char *extraction = strstr(client, c); //String to hold the rest of *client, used to find clientName
            strncpy(clientName, client, extraction-client);

            //Finding function to compute
            if (strcmp(foo, "avgSubtree") == 0)
            {
                pthread_rwlock_rdlock(&rwlock);
                float avg = avgSubtree(root);
                printf("%savgSubtree = %f\n", clientName, avg);
                pthread_rwlock_unlock(&rwlock);
            } else if (strcmp(foo, "countNodes") == 0)
            {
                pthread_rwlock_rdlock(&rwlock);
                int total = countNodes(root);
                printf("%scountNodes = %d\n", clientName, total);
                pthread_rwlock_unlock(&rwlock);
            } else if(strcmp(foo, "addNode") == 0)
            {
                pthread_rwlock_wrlock(&rwlock);
                arg = strtok_r(NULL, "\n", &arg);
                int real_arg = atoi(arg);
                root = addNode(root, real_arg);
                printf("%sinsertNode %s\n", clientName, arg);
                pthread_rwlock_unlock(&rwlock);

            } else if(strcmp(foo, "removeNode") == 0)
            {
                pthread_rwlock_wrlock(&rwlock);
                arg = strtok_r(NULL, "\n", &arg);
                int real_arg = atoi(arg);
                root = removeNode(root, real_arg);
                printf("%sdeleteNode %s\n", clientName, arg);
                pthread_rwlock_unlock(&rwlock);
            }
        }
    }
    fclose(fp);
    return NULL;
}

/*
 * -------------------- ARCHIVE --------------------
 */

/*
 * In downtime()
 */

/*while (count < 4)
{ //Program ends after 3 downtimes.
    sleep(2);
    pthread_rwlock_wrlock(&rwlock);
    root_balanced = balanceTree(root);
    //freeSubtree(root); //-- causes sigabrt
    pthread_rwlock_unlock(&rwlock);
    count++;
}*/

/*
 * In ServeClient()
 */

//printf("LINE: %s", line);

//printf("%s", foo);
//printf("%s", arg);
/*if (strcmp(line, foo) == 0)
{
//                printf("%s   %s", foo, line);
//                printf("strcmp(foo, line) == 0 REACHED\n");
    if (strcmp("avgSubtree", foo) == 0)
    {
        pthread_rwlock_rdlock(&rwlock);
        float avg = avgSubtree(root);
        clientName = strtok_r(client, "_", &commands);
        printf("%savgSubtree = %f\n", clientName, avg);
        pthread_rwlock_unlock(&rwlock);
    }
    else if(strcmp("countNodes", foo) == 0)
    {
        pthread_rwlock_rdlock(&rwlock);
        int total = countNodes(root);
        clientName = strtok_r(client, "_", &commands);
        printf("%scountNodes = %d\n", clientName, total);
        pthread_rwlock_unlock(&rwlock);
    }
}
else
{
    printf("ELSE REACHED\n");
    arg = strtok_r(NULL, "\n", &arg);
    if (strcmp("addNode", foo) == 0)
    {
        pthread_rwlock_wrlock(&rwlock); //Lock thread
//                    printf("addNode Lock\n");
        int real_arg = atoi(arg);
        root = addNode(root, real_arg);
        clientName = strtok_r(client, "_", &commands);
        printf("%sinsertNode %s\n", clientName, arg);
        pthread_rwlock_unlock(&rwlock);
    } else if (strcmp("removeNode", foo) == 0)
    {
        pthread_rwlock_wrlock(&rwlock);
//                    printf("removeNode Lock\n");
        int real_arg = atoi(arg);
        root = removeNode(root, real_arg);
        clientName = strtok_r(client, "_", &commands);
        printf("%sdeleteNode %s\n", clientName, arg);
        pthread_rwlock_unlock(&rwlock);
    }
}*/

//printf("%s", arg);