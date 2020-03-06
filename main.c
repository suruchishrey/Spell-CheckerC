#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
typedef struct letter_node_type{
    char data;
    struct letter_node_type* right;
}letter_node;
typedef struct dict_node_type{
    struct dict_node_type* down;
    struct letter_node_type* right;
}dict_node;
typedef enum{FALSE,TRUE} boolean;
typedef struct word_type{
    char data[40];//array of string
    int freq;
    struct word_type* next;
}wordNode;
boolean areEqual(letter_node* aptr,letter_node* bptr);

wordNode* MakeWordNode(char* word)
{
    wordNode* wptr=(wordNode*)malloc(sizeof(wordNode));
    strcpy(wptr->data,word);
    wptr->freq=1;
    return wptr;
    
}

boolean areSamebutDifferentCase(letter_node* aptr,letter_node* bptr)
{
    letter_node* ptr1,*ptr2;
    int flag=0;
    boolean retval;
    ptr1=aptr;ptr2=bptr;
    if(abs(ptr1->data-ptr2->data) ==32)
    {
        if(areEqual(ptr1->right,ptr2->right)==TRUE)
        {
            retval=TRUE;
        }
        else{
            retval=FALSE;
        }
    }
    else{
        retval=FALSE;
    }
    return retval;
}
boolean areEqual(letter_node* aptr,letter_node* bptr)
{
    int flag=0;
    boolean retval;
    letter_node* ptr1,*ptr2;
    ptr1=aptr;ptr2=bptr;
    while(ptr1 && ptr2)
    {
        if(ptr1->data!=ptr2->data)
        {
            flag=1;
        }
    }
    if(ptr1==NULL && ptr2==NULL && flag==0)
    {
        retval=TRUE;
    }
    else{
        retval=FALSE;
    }
    return retval;

}
letter_node* MakeNode(char letter)
{
    letter_node* lptr;
    lptr = (letter_node*)malloc(sizeof(letter_node));
    lptr->data = letter;
    lptr->right=NULL;
    return lptr;
}
dict_node* MakeDictNode()
{
    dict_node* lptr;
    lptr = (dict_node*)malloc(sizeof(dict_node));
    lptr->down=NULL;
    lptr->right=NULL;
    return lptr;
}

letter_node* wordToList(char* word){
    int len = strlen(word);
    letter_node* ptr,*lptr=NULL;
    for(int i=len-1;i>=0;i--)
    {
        ptr=MakeNode(word[i]);
        ptr->right =lptr;
        lptr=ptr;
    }
    return lptr;

}
void InsertAfter(dict_node* ptr,letter_node* wordptr)
{
    dict_node *nptr;
    nptr = (dict_node*)malloc(sizeof(dict_node));
    nptr->down = ptr->down;
    nptr->right = wordptr;
    ptr->down = nptr;
}

dict_node* create_dict(char* word,dict_node** dictpptr){
    letter_node* wordptr;
    int flag=0;
    dict_node* dictptr=*dictpptr;
    dict_node *ptr,*prev=NULL;
    wordptr=wordToList(word);
    if(dictptr==NULL) //Inserting the first node in the dict(DICT EMPTY)
    {
        ptr=MakeDictNode();
        ptr->down=NULL;
        ptr->right=wordptr;
        *dictpptr=ptr;
    }
    else//DICT NOT EMPTY
    {
        //first check whether the word given has a different case present already
        ptr=dictptr;
        while(ptr!=NULL && flag==0)
        {
            prev=ptr;
            if(areSamebutDifferentCase(ptr->right,wordptr))
            {
                flag=1;
            }
            else{ //this else ensures that if any word matches i.e is different case then
                    //ptr would not go down and stay on the matched word only. 
                ptr=ptr->down;
            }
            
        }
        //ptr contains the value of the matched case
        if(flag==1)
        {
            if((wordptr->data - (ptr->right->data))==32)
            {
                //word should be inserted after ptr
                InsertAfter(ptr,wordptr);
            }
            else{
                //word should be inserted before ptr
                if(prev==NULL)
                {
                    dict_node* dptr = MakeDictNode();
                    dptr->down=dictptr;
                    *dictpptr = dptr;
                }
                else{
                    InsertAfter(prev,wordptr);
                }
                
            }
        }
    }
    
}
boolean presentInDictionary(char* word,dict_node* dictptr){
    letter_node* wordptr = wordToList(word);
    boolean retval=FALSE;int flag=0;
    dict_node* ptr=dictptr;
    while(ptr!=NULL && flag==0)
    {
        if(areEqual(wordptr,ptr->right)){
            flag=1;
            retval=TRUE;
        }
        else{
            ptr=ptr->down;
        }
    }
    return retval;
}
void readFileAndRemovePunctuation(){
    char ch, file_name[25];
    FILE *fp,*result;
   fp = fopen("doc.txt", "r"); // read mode
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
            
        }  
        fclose(fp);
        fclose(result);
   }
}

boolean IsMisspelled(char* word,dict_node* dictptr){
    boolean retval = TRUE;
    if(presentInDictionary){
        retval = FALSE;
    }
    return retval;
}

wordNode* insert_mis(char* word,wordNode* endptr){
    //INSERTING AT START
    //RETURNS THE ENDPOINTER ITSELF
    wordNode* retval=endptr;wordNode* lptr;
    wordNode* wptr=(wordNode*)malloc(sizeof(wordNode));
    wptr = MakeWordNode(word);
    if(endptr==NULL)
    {
        wptr->next=wptr;
        retval = wptr;
    }
    else{
        lptr = endptr->next;
        wptr->next = lptr;
        endptr->next=wptr;
    }
    return retval;

}

int main()
{
    readFileAndRemovePunctuation();
    return 0;
}
