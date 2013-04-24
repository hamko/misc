#include <stdio.h>
#include "robolib_matrix.h"

#define IMGX 6
#define IMGY 6
#define MAT_DEG (IMGX*IMGY)
#define FILE_NUM 4
#define CYCLE_NUM 100

typedef robolib_matrix mat_t;
    
mat_t renew_field(mat_t* input, mat_t* output, mat_t* corr_mat)
{
    mat_t* u = robolib_column_vector_alloc(MAT_DEG);

    u = robolib_matrix_product(corr_mat, input, u);
    int i;
    for (i = 0; i < MAT_DEG; i++) {
        if (robolib_vector_get(u, i) < 0)
            robolib_vector_set(output, i, -1);
        else
            robolib_vector_set(output, i, 1);
    } 
}

void plus_wmat(robolib_matrix* mat, FILE* fp)
{
    int i = 0;
    int c;
    mat_t* tmpimg = robolib_column_vector_alloc(MAT_DEG);
    mat_t* tmpimg_t = robolib_vector_alloc(MAT_DEG);
    mat_t* prod = robolib_matrix_alloc(MAT_DEG, MAT_DEG);
    mat_t* inc = robolib_matrix_alloc(MAT_DEG, MAT_DEG);

    while ((c = fgetc(fp)) != EOF && i < MAT_DEG) {
        if (!(c == '0' || c == '1'))
            continue;
        robolib_matrix_set(tmpimg, i, 0, c - '0' == 0 ? -1 : 1);
        i++;
    }

    tmpimg_t = robolib_matrix_trans(tmpimg, tmpimg_t);

    prod = robolib_matrix_product(tmpimg, tmpimg_t, prod);

    inc = robolib_matrix_linear_comb(1, mat, 1, prod, inc);
    mat = robolib_matrix_copy(inc, mat);

    robolib_matrix_free(inc);
    robolib_matrix_free(prod);
    robolib_matrix_free(tmpimg_t);
    robolib_matrix_free(tmpimg);
}


mat_t* calc_wmat(robolib_matrix* mat)
{
    FILE* fp;
    char* filename[FILE_NUM] = {"circle.txt", "cross.txt", "triangle.txt", "square.txt"};
    int i;

    for (i = 0; i < FILE_NUM; i++) {
        if ((fp = fopen(filename[i], "r")) == NULL)
            fprintf(stderr, "no file.");
        plus_wmat(mat, fp);
        fclose(fp);
    }

    // 対角成分を0に
    for (i = 0; i < MAT_DEG; i++) {
        robolib_matrix_set(mat, i, i, 0);
    } 

    return mat;
}

mat_t* get_inputmat(mat_t* input)
{
    FILE *fp;
    int c;
    int i = 0;
    if ((fp = fopen("input.txt", "r")) == NULL)
        return;

    
    while ((c = fgetc(fp)) != EOF && i < MAT_DEG) {
        if (!(c == '0' || c == '1'))
            continue;
        robolib_matrix_set(input, i, 0, c == '0' ? -1 : 1);
        i++;
    }
    
    fclose(fp);

    return input;
}

    
int main(void)
{
    int i, j;
    mat_t* field_w = robolib_matrix_alloc(MAT_DEG, MAT_DEG);
    mat_t* tmp = robolib_column_vector_alloc(MAT_DEG);
    mat_t* input = robolib_column_vector_alloc(MAT_DEG);
    
    input = get_inputmat(input);

    field_w = calc_wmat(field_w);

    // ホップフィールドネットワークを更新する
    for (i = 0; i < CYCLE_NUM; i++) {
        renew_field(input, tmp, field_w);
        tmp = robolib_matrix_copy(tmp, input);
    }

    printf("もしかして：\n");
    for (i = 0; i < IMGX; i++) {
        for (j = 0; j < IMGY; j++) {
            printf("%d", (int)input->val[i*IMGY+j] == 1 ? 1 : 0);
        }
        printf("\n");
    }

    robolib_matrix_free(input);
//    robolib_matrix_free(tmp);
    robolib_matrix_free(field_w);

    return 0;
}
