#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>

typedef struct word_type{
    char data[40];          //character string
    int freq;
    struct word_type* next;
}wordNode;

typedef struct dict_node_type{
    struct dict_node_type* down;
    wordNode* right;
    char letter;
}dict_node;

typedef struct dict_tag{
    dict_node*head;
}Dictionary;

typedef enum{FALSE,TRUE} boolean;

typedef struct MRU_tag{
    wordNode*top;
}MRU;
typedef struct MIS_tag{
    wordNode*endptr;
}MIS_List;

wordNode* MakeWordNode(char* word)
{
    wordNode* wptr=(wordNode*)malloc(sizeof(wordNode));
    if(wptr!=NULL)
    {
        strcpy(wptr->data,word);
        wptr->freq=0;
        wptr->next=NULL;
    }
    
    return wptr;
    
}

//Creation of Dictionary part and its associated functions
void MakeDictNodes(Dictionary*dict_ptr)
{
    dict_node* lptr,*ptr;
    lptr = (dict_node*)malloc(sizeof(dict_node));
   
    dict_ptr->head=lptr;
    int i=1;
    char letter='a';
    lptr->down=NULL;
    lptr->right=NULL;
    lptr->letter=letter;
    while(i<=26)
    {
        
        letter=letter+1;
        ptr = (dict_node*)malloc(sizeof(dict_node));
        
        ptr->right=NULL;
        ptr->letter=letter;
        ptr->down=NULL;
        lptr->down=ptr;
    
        lptr=lptr->down;
        
        i++;
        
    }
    
    lptr->down=NULL;
    lptr->right=NULL;
    
}

void createDictionary(Dictionary*dict_ptr)
{
    dict_ptr->head=NULL;
    MakeDictNodes(dict_ptr);
}

void TraverseDictionary(Dictionary*dictptr)
{
    dict_node*ptr=dictptr->head;
    int i=0;
    wordNode*wptr;
    while(ptr!=NULL)
    {
        printf("\n");
        printf(" %c -->",ptr->letter);
        wptr=ptr->right;
        while(wptr!=NULL)
        {
            printf(" %s , ",wptr->data);
            wptr=wptr->next;
        }
        ptr=ptr->down;
        i++;
    }    
    printf("\ncount=%d",i);
}

int newstrcmp(char* word1,char* word2)
{
    int res,flag=1;
    int len1,len2,diff;
    len1=strlen(word1);
    len2=strlen(word2);
    if(len1==len2)
     {   for(diff=0;word1[diff]!='\0';++diff)
        {
            if((abs(word1[diff]-word2[diff])!=32) && (abs(word1[diff]-word2[diff])!=0))
            {
                flag=0;
            }
        }
        if(flag){                                 //Strings are equal but in different case
            res=strcmp(word1,word2);
        }
     }
     if(!flag){   
        if(abs(word1[0]-word2[0])==32)             //Strings are diff but have different case in first letter so compare from second letter
        {
            word1++;word2++;
            res=strcmp(word1,word2);
        }
        else
        {
             res=strcmp(word1,word2);
        }
     }
    return res;
}

//Function for inserting word in dictionary's one node in increasing order
void InsertinDictNode(dict_node*dict_ptr,wordNode*wordptr)
{
    dict_node*ptr=dict_ptr;
    int len,res;
    char word[40],word1[40];
    int diff;
    strcpy(word,wordptr->data);
    if(ptr->right==NULL)
    {
        ptr->right=wordptr;
    }
    else
    {
        wordNode * dptr=ptr->right;
        wordNode*prev=NULL;
        strcpy(word1,dptr->data);
        res=newstrcmp(word,word1);
        if(res<0)
        {
            ptr->right=wordptr;
            wordptr->next=dptr;
        }
        else
        {
            while(res>0 && dptr->next!=NULL)                               //keep on comparing each word with the word to be inserted 
            {
                prev=dptr;
                dptr=dptr->next;
                strcpy(word1,dptr->data);
                res=newstrcmp(word,word1);

            }
            if(res<0)                                                      //if next word is greater than word to be inserted then insert it
            {
                prev->next=wordptr;
                wordptr->next=dptr;
            }
            else if(dptr->next==NULL)
            {
                dptr->next=wordptr;
            }
        }
        
        
    }
    
}

dict_node* create_dict(char* word,Dictionary*dict_ptr){
    wordNode* wordptr;
    int flag=0;
    dict_node* dictptr=dict_ptr->head;
    dict_node *ptr;
    wordptr=MakeWordNode(word);
    
        char ch=tolower(word[0]);
        int chvalue=word[0];
        //look for the letter node
        ptr=dictptr;
        while(ptr!=NULL)
        {
            if(ptr->letter==ch)
            {
                InsertinDictNode(ptr,wordptr);
            }
            else if(ch>=48 && ch<=57 && ptr->down==NULL)
            {
                InsertinDictNode(ptr,wordptr);
            }
            ptr=ptr->down;
        }
}

boolean presentInDictionary(char* word,Dictionary* dictptr)
{
    wordNode* wordptr = MakeWordNode(word);
    boolean retval=FALSE;
    int flag=0,flag1=0;
    dict_node*dptr,*ptr=dictptr->head;
    int check;
    char word1[40];
    char ch=tolower(word[0]);
    while(ptr!=NULL && flag==0)
    {
        if(ptr->letter==ch || ((ch>=48 && ch<=57) && ptr->down==NULL))
        {
            flag=1;
            dptr=ptr;                           //word can be present in this node only
            wordNode*wptr=dptr->right;
            while(wptr!=NULL && flag1==0)
            {
                strcpy(word1,wptr->data);
                check=newstrcmp(word,word1);
                if(check==0)
                {
                    retval=TRUE;
                    flag1=1;
                }
                else
                {
                    wptr=wptr->next;
                }
            }
            
        }
       
        ptr=ptr->down;
    }
    
    return retval;
}
//Dictionary part over

//Creation of MRU and its associated functions
void create_MRU(MRU*mruptr)
{
    mruptr->top=NULL;
}

int len_mru(MRU*mruptr)                             //function to calculate no of nodes currently present in MRU
{
    wordNode*wptr=mruptr->top;
    int len=0;
    while(wptr!=NULL)
    {
        len++;
        wptr=wptr->next;
    }
    return len;
}

boolean is_present(MRU*mruptr,char*word)
{
    wordNode*wptr=MakeWordNode(word);
    wordNode*ptr=mruptr->top;
    boolean retval=FALSE;
    int flag=0;
    while(ptr!=NULL && !flag)
    {
        if(newstrcmp(ptr->data,word)==0)
        {
            flag=1;
            retval=TRUE;
        }
        else{
            ptr=ptr->next;
        }
        
    }
    return retval;
}

void increment(MRU*mruptr,wordNode*wptr)
{
    wordNode*ptr = mruptr->top,*prev=NULL;
    int flag=1;
    while(ptr!=NULL && flag)
    {
        if(strcmp(wptr->data,ptr->data)==0)
        {
            ptr->freq++;
            flag=0;
        }
        else{
            prev=ptr;
            ptr=ptr->next;
        }
        
    }
    //prev contains prev element of matched word
    if(prev!=NULL)
    {
        prev->next = ptr->next;
        ptr->next=mruptr->top;
        mruptr->top = ptr;
    }
    
}

void DeleteLeastFreq(MRU* mruptr)
{
    wordNode*ptr=mruptr->top;
    wordNode*delptr=ptr,*prev=NULL;
    while(ptr!=NULL)
    {
        if(ptr->freq<delptr->freq)
        {
            delptr=ptr;
        }
            ptr=ptr->next;
    }
    ptr=mruptr->top;
    
    while(ptr!=delptr)
    {
        prev=ptr;
        ptr=ptr->next;
    }
    if(prev==NULL)
    {
        mruptr->top=ptr->next;
    }
    else{
        prev->next=ptr->next;
    }
    free(delptr);
}

void insert_MRU(MRU*mruptr,char*word,Dictionary*dict_ptr)
{
    wordNode*wptr=MakeWordNode(word);

    boolean res;
    wordNode *ptr1,*ptr=mruptr->top;

    res=presentInDictionary(word,dict_ptr);
    
   
    if(res)
    {
        if(mruptr->top==NULL)
        {
            
            mruptr->top=wptr;
            wptr->freq=1;
            wptr->next=NULL;
        }
        else
        {
            
            if(is_present(mruptr,word))
            {
                
                increment(mruptr,wptr);
            }
            else
            {
                wptr->freq=1;
                if(len_mru(mruptr)<10)                         //if length of MRU is<10 and its not already present then insert the new node at the start
                {
                    wptr->next=mruptr->top;
                    mruptr->top=wptr;
                }
                else if(len_mru(mruptr)==10)
                {
                    DeleteLeastFreq(mruptr);
                    wptr->next=mruptr->top;
                    mruptr->top=wptr;
                }
            }
            
            
        }
    }
    
    
}
//display() is incomplete rn!!!
void display(MRU*mruptr)
{
    printf("----------------------------------------------------------------------------------------------------------\n");
    wordNode*ptr=mruptr->top;
    while(ptr!=NULL)
    {
        printf(" %s-->%d ,",ptr->data,ptr->freq);
        ptr=ptr->next;
    }
    printf("-----------------------------------------------------------------------------------------------------------\n");

}
//MRU part done

//Creation of Misspelled List and its associated functions
void create_MIS_List(MIS_List*mlptr)                    //Creating an empty list
{
    mlptr->endptr=NULL;
}

boolean is_present_MISList(MIS_List*mlptr,char*word)
{
    boolean retval=FALSE;
    int flag=1;
    wordNode*ptr=mlptr->endptr->next;
    if(strcmp(mlptr->endptr->data,word)==0)                  //check if the last node is the required one
    {
        retval=TRUE;
    }
    else                                                    
    {
        /* code */
        while(ptr!=mlptr->endptr && flag)                //else Iterate over the CLL to find 
        {
            if(strcmp(ptr->data,word)==0)
            {
                retval=TRUE;
                ptr->freq++;
                flag=0;
            }
            else
            {
                ptr=ptr->next;
            }
            
        }
    } 
    return retval;
}

void insert_mis(char* word,MIS_List*mlptr){

    wordNode* ptr=mlptr->endptr;
    wordNode*lptr,*wptr = MakeWordNode(word);
    
    if(mlptr->endptr==NULL)                                //INSERTING AT START and incrementing the frequency simultaneously
    {
        wptr->next=wptr;
        mlptr->endptr=wptr;
        wptr->freq++;
    }
    else if(is_present_MISList(mlptr,word))
    {
        wptr->freq++;
    }
    else{
        lptr=ptr->next;
        mlptr->endptr->next=wptr;
        wptr->next=lptr;
        mlptr->endptr=wptr;
        wptr->freq++;
    }

}

void display_mis(MIS_List*mlptr)
{
    wordNode*ptr=mlptr->endptr->next;
    printf("--------------------------------------------------------------------------------------------------\n");
    if(ptr!=NULL)
    {
        do{
            printf(" %s-->%d ,",ptr->data,ptr->freq);
            ptr=ptr->next;
        }
        while(ptr!=mlptr->endptr->next);
        
        printf("\n");
    }
    printf("---------------------------------------------------------------------------------------------------\n");
    
}
//Misspelled List done

void readFileAndRemovePunctuation(char* fl){
    char ch, file_name[25];
    FILE *fp,*result;
   fp = fopen(fl, "r"); // read mode
   result = fopen("result.txt","w");
   if (fp != NULL)
   {
        while((ch = fgetc(fp)) != EOF)
        {
            if(ch>=65 || ch==32||(ch>=48 && ch<=57))
            //WE CAN USE THIS LOGIC TO GENERATE ANOTHER FILE WITHOUT THE PUNCTUATION MARKS.
            {
                fprintf(result,"%c", ch);
            }
            else{
                ch=' ';
                fprintf(result,"%c", ch);
            }
            
        }  
        fclose(fp);
        fclose(result);
   }
}

void readResultFile(Dictionary* dictptr,MRU*mruptr,MIS_List*mlptr){
    FILE* result;
    char ch;char word[40];
    result = fopen("result.txt","r");
    while((ch!=EOF))
    {
        
        fscanf(result,"%s",word);                   //we will get individual words in word.
        
        if(!(is_present(mruptr,word)))
        {
            if(!(presentInDictionary(word,dictptr)))
            {
                //printf("\nnotPresent in MRU!! misspelled!! word %s",word);
                insert_mis(word,mlptr);
            }
            else{
                insert_MRU(mruptr,word,dictptr);
            }
        }
        else{
            //Search Mru for that word and increase freq
            insert_MRU(mruptr,word,dictptr);
        }
         
        ch=fgetc(result);
    }


}



/* Function to bubble sort the given linked list */
void bubbleSort(wordNode *start); 
  
/* Function to swap data of two nodes a and b*/
void swapfandword(wordNode *a, wordNode *b); 
  
/* Function to print nodes in a given linked list */
void printList(wordNode *start); 
  
  
   
/* Function to print nodes in a given linked list */
/*void printSortedMRU(wordNode *start) 
{ 
    wordNode *temp = start; 
    printf("\n"); 
    while (temp!=NULL) 
    { 
        printf("%d ", temp->freq); 
        temp = temp->next; 
    } 
} */
  
/* Bubble sort the given linked list */
void bubbleSort(wordNode *start) 
{ 
    int swapped, i; 
    wordNode *ptr1; 
    wordNode *lptr = NULL; 
  
    /* Checking for empty list */
    if (start == NULL) 
        return; 
  
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->next != lptr) 
        { 
            if (strcmp(ptr1->data,ptr1->next->data)>0) 
            {  
                swapfandword(ptr1,ptr1->next);
        
                swapped = 1; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } 
    while (swapped); 
} 
  
/* function to swap data of two nodes a and b*/
void swapfandword(wordNode *a, wordNode *b) 
{ 
    int temp = a->freq; 
    a->freq = b->freq; 
    b->freq = temp;
    char tempw[40];
    strcpy(tempw,a->data);
    strcpy(a->data,b->data);
    strcpy(b->data,tempw);
} 

void display_MRU_Sorted(MRU* mruptr)
{
    bubbleSort(mruptr->top);
    display(mruptr);
}

void display_mis_Sorted(MIS_List* mis)
{
    wordNode* lptr=mis->endptr->next;
    mis->endptr->next=NULL;
    bubbleSort(lptr);
    wordNode* ptr;
    ptr=lptr;
    while(ptr->next!=NULL)
    {
        ptr=ptr->next;
    }
    ptr->next=lptr;
    mis->endptr=ptr;

    display_mis(mis);
}

/*searchAndDeleteLeastFreq(MRU* mruptr)
{
    //Least frequency will always be the last element if we move every found element in the beginning
    wordNode* ptr,*prev=NULL;
    int min=__INT16_MAX__;
    ptr=mruptr->top;
    while(ptr!=NULL)
    {
        if(ptr->freq<min)
        {
            min
        }
    }
}*/
//Whenever a word in mru is encountered ,put the word in beginning of MRU
/*void MRUhit(MRU* mruptr,char* word){
    wordNode* ptr;
} */

int main(int argc,char* argv[])
{
    
    char fl[20];
    strcpy(fl,argv[1]);
    Dictionary new_dict;
    Dictionary*dict_ptr=&new_dict;
    createDictionary(dict_ptr);
    //MakeDictNodes(dict_ptr);
    MRU MRU_List;
    MRU*mruptr=&MRU_List;
    create_MRU(mruptr);
    MIS_List MIS_data;
    MIS_List*mlptr=&MIS_data;
    create_MIS_List(mlptr);
    create_dict("Abc",dict_ptr);
    //insert_MRU(mruptr,"Abc",dict_ptr);
    
    /*create_dict("Gandhi",dict_ptr);
    create_dict("Father",dict_ptr);
    create_dict("Nation",dict_ptr);
    create_dict("bhi",dict_ptr);
    create_dict("def",dict_ptr);
    insert_MRU(mruptr,"def",dict_ptr);
    create_dict("1948",dict_ptr);
    create_dict("jkl",dict_ptr);
    insert_MRU(mruptr,"Abc",dict_ptr);
    create_dict("mno",dict_ptr);
    create_dict("xyz",dict_ptr);
    create_dict("Tip",dict_ptr);
    create_dict("tip",dict_ptr);
    create_dict("The",dict_ptr);
    create_dict("the",dict_ptr);
    */
    FILE* fp;
    fp=fopen("engmix.txt","r");
    char ch;
    char word[40];
    while((ch=fgetc(fp))!=EOF)
    {
        fscanf(fp,"%s",word);
        create_dict(word,dict_ptr);
    }
    readFileAndRemovePunctuation(fl);
    readResultFile(dict_ptr,mruptr,mlptr);

   // TraverseDictionary(dict_ptr);
    printf("\nMRU!!!\n");
    display(mruptr);
    printf("\nMisspelled list!!!\n");
    display_mis(mlptr);
    printf("\nSorted MRU!!!\n");
    display_MRU_Sorted(mruptr);
    printf("\nSorted Misspelled list!!!\n");
    display_mis_Sorted(&MIS_data);
}
