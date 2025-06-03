#include "../include/author.h"
#include <string.h>
#include <stdlib.h>


Author* author_create(const char *name, const char *email){
    Author *author=(Author*)malloc(sizeof(Author));
    if(author == NULL){
        return NULL;
    }

    author->author_id=0;
    strncpy(author->name, name, sizeof(author->name) - 1);
    author->name[sizeof(author->name) - 1] = '\0';
    strncpy(author->email, email, sizeof(author->email) - 1);
    author->email[sizeof(author->email) - 1] = '\0';
    author->created_at=time(NULL);
    author->updated_at = author->created_at;

    return author;
}


void author_free(Author *author){
    if(author != NULL){
        free(author);
    }
}

int author_save(Database *db, Author* author){
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[2];

    const char *query = "INSERT INTO authors (name, email) VALUES (?,?)";
    stmt= mysql_stmt_init(db->conn);
    if(!stmt) return -1;

    if(mysql_stmt_prepare(stmt, query, strlen(query))){
        mysql_stmt_close(stmt);
        return -1;
    }

    memset(bind, 0, sizeof(bind));
    
    bind[0].buffer_type= MYSQL_TYPE_STRING;
    bind[0].buffer= author->name;
    bind[0].buffer_length=strlen(author->name);
    
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = author->email;
    bind[1].buffer_length = strlen(author->email);

    if(mysql_stmt_bind_param(stmt,bind)){
        mysql_stmt_close(stmt);
        return -1;
    }

    if(mysql_stmt_execute(stmt)){
        mysql_stmt_close(stmt);
        return -1;
    }

    mysql_stmt_close(stmt);
    return 0;




}


int author_update(Database *db, Author *author) {
    MYSQL_STMT *stmt = mysql_stmt_init(db->conn);
    if (!stmt) {
        fprintf(stderr, "Failed to initialize statement\n");
        return -1;
    }

    // Include updated_at in the query
    const char *query_str = "UPDATE authors SET name=?, email=?, updated_at=? WHERE author_id=?";
    
    if (mysql_stmt_prepare(stmt, query_str, strlen(query_str)) != 0) {
        fprintf(stderr, "Failed to prepare statement: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    // Update timestamp
    author->updated_at = time(NULL);

    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));

    // Bind name (no escaping needed with prepared statements)
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = author->name;
    bind[0].buffer_length = strlen(author->name);
    bind[0].is_null = 0;

    // Bind email
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = author->email;
    bind[1].buffer_length = strlen(author->email);
    bind[1].is_null = 0;

    // Bind updated_at (timestamp)
    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = &author->updated_at;
    bind[2].is_null = 0;

    // Bind author_id (assuming it's an integer)
    bind[3].buffer_type = MYSQL_TYPE_LONG;
    bind[3].buffer = &author->author_id;
    bind[3].is_null = 0;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        fprintf(stderr, "Failed to bind parameters: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        fprintf(stderr, "Failed to execute statement: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    mysql_stmt_close(stmt);
    return 0; // Success
}
int author_delete(Database *db, int author_id){
    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM authors WHERE author_id= %d", author_id);
    return db_execute_query(db, query);
}

Author* author_find_by_id(Database *db, int author_id){
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM authors WHERE author_id=%d", author_id);

    MYSQL_RES *result= db_execute_select(db, query);
    if(result == NULL){
        return NULL;
    }

    MYSQL_ROW row=mysql_fetch_row(result);
    if(row== NULL){
        db_free_result(result);
        return NULL;
    };

    Author *author = (Author*)malloc(sizeof(Author));
    if(author == NULL) {
        db_free_result(result);
        return NULL;
    }

    author->author_id=atoi(row[0]);
    strncpy(author->name, row[1], sizeof(author->name)-1);
    strncpy(author->email, row[2], sizeof(author->email)-1);

    db_free_result(result);
    return author;

}

Author* author_find_by_name(Database *db, const char *name){
    char query[255];
    snprintf(query, sizeof(query), "SELECT * FROM authors WHERE name = '%s',", name);
    MYSQL_RES *result =db_execute_select(db, query);
    if(result == NULL){
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if(row == NULL){
        db_free_result(result);
        return NULL;
    }
    Author *author = (Author*)malloc(sizeof(Author));
    if(author== NULL){
        db_free_result(result);
        return NULL;
    }

    author->author_id=atoi(row[0]);
    strncpy(author->name, row[1], sizeof(author->name)-1);
    strncpy(author->email, row[2], sizeof(author->email)-1);

    db_free_result(result);
    return author;

}


AuthorList* author_list_create(int initial_capacity){
    AuthorList *list=(AuthorList*)malloc(sizeof(Author));
    if(list == NULL){
        return NULL;
    }

     list->count=0;
     list->capacity= initial_capacity;
     return list;
}

void author_list_free(AuthorList *list){
    if(list != NULL){
        for(int i=0;i<list->count;i++){
            author_free(list->authors[i]);
        }
        free(list->authors);
        free(list);
    }
}

int author_list_add(AuthorList *list, Author *author){
    if(list->count >= list-> capacity){
        int new_capacity = list->capacity *2;
        Author **new_authors= (Author**)realloc(list->authors, sizeof(Author*) *  new_capacity);
        if(new_authors == NULL ){
            return 0;
        }
        list->authors=new_authors;
        list->capacity=new_capacity;

    }
    list->authors[list->count++]=author;
    return 1;
}

AuthorList* author_list_all(Database *db){
    const char *query ="SELECT * FROM authors";
    MYSQL_RES *result= db_execute_select(db, query);
    if(result == NULL ){
        return NULL;
    }

    AuthorList *list = author_list_create(10);
    if(list == NULL){
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row= mysql_fetch_row(result)) != NULL){
        Author *author= (Author*)malloc(sizeof(Author));
        if(author == NULL){
            continue;
        }
        author->author_id=atoi(row[0]);
        strncpy(author->name, row[1], sizeof(author->name)-1);
        strncpy(author->email, row[2], sizeof(author->email)-1);

        author_list_add(list, author);

    }
    db_free_result(result);
    return list;
}