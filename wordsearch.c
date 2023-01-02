#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>

int is_alpha(char ch)
{
    return ((ch>=65 && ch<=90) || (ch>=97 && ch<=122));
}

void read_word(char* word, FILE *fp)
{
    int i=0;
    char ch;

    while((ch=fgetc(fp))!=EOF && !is_alpha(ch));

    while(ch!=EOF && is_alpha(ch))
    {
        word[i++] = ch;
        ch = fgetc(fp);
    }
    word[i] = '\0';
}

int count_occurence(FILE *fp, const char* word)
{
    int cnt = 0;
    char temp[100];

    while(!feof(fp))
    {
        read_word(temp, fp);
        if(strcmp(temp, word)==0)
            cnt++;
    }

    return cnt;
}

int main()
{
    // open words file
    FILE* fp = fopen("words.txt", "r");

    double t = omp_get_wtime();
    
    // determine count of word 'apple'
    int count = count_occurence(fp, "apple");

    // print results
    printf("%d occurences, time taken=%lf\n", count, omp_get_wtime()-t);
}