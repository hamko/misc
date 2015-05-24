#include <sqlite3.h>
#include <stdio.h>

// 抽出結果が返るコールバック関数
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++)
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    return SQLITE_OK;
}

int main(int argc, char** argv)
{
    sqlite3 *db;
    char *zErrMsg = 0;

    // データベースファイルを新規生成
    int rc = sqlite3_open("Sample.db", &db);

    // テーブル生成SQL文
    char create_sql[] = "CREATE TABLE sample ( "
        "               id     INTEGER PRIMARY KEY, "
        "               worker TEXT    NOT NULL,    "
        "               place  TEXT    NOT NULL     "
        "             )                             ";

    // テーブルを生成
    rc = sqlite3_exec(db, create_sql, 0, 0, &zErrMsg);

    // データ追加SQL文
    char insert_sql[] = "INSERT INTO sample ( id, worker, place )"
        "            values (%d, '%s', '%s')     ";

    // 追加SQL文を生成
    char insert_record[3][256];
    sprintf( insert_record[0], insert_sql, 0, "IKD"  , "Tokyo" );
    sprintf( insert_record[1], insert_sql, 1, "Maru" , "Kanagawa" );
    sprintf( insert_record[2], insert_sql, 2, "Nacky", "Hukuoka" );

    // データ追加
    int i;
    for( i = 0; i < 3; i++ )
        rc = sqlite3_exec(db, insert_record[i], 0, 0, &zErrMsg);

    // "sample"テーブルから"worker"を抽出して列挙
    rc = sqlite3_exec(db, "SELECT worker FROM sample", callback, 0, &zErrMsg);

    // データベースを閉じる
    sqlite3_close(db);
    return 0;
}
