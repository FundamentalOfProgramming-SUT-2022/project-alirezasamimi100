#include "mainHeaders.h"

void createfile(char** sptr) {
    char* ptr = *sptr;
    const char end[] = {' ', '\"'};
    int mode = 0;
    while(*ptr == ' ') ++ptr;
    if(*(ptr + 7) == '\"') ptr++, mode = 1;
    ptr += 8;
    char path[FILE_PATH_SIZE], *cur = path;
    while(*ptr != end[mode] && *ptr != '\n' && *ptr != '\0') {
        *cur = *ptr;
        if(*cur == '/') {
            *cur = '\0';
            mkdir(path, 0755);
            *cur = '/';
        }
        ++cur;
        ++ptr;
    }
    if(*ptr == end[mode]) ++ptr;
    *cur = '\0';
    FILE* file = fopen(path, "r");
    if(!file) {
        file = fopen(path, "w");
    } else {
        reterr("File Already Exists!");
    }
    fclose(file);
    *sptr = ptr;
}

void insertstr(char** sptr, char* pipeString) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE], str[MAX_STRING_SIZE];
    while(*ptr == ' ') ++ptr;
    ptr += 7;
    getinstr(&ptr, path, 0);
    while(*ptr == ' ') ++ptr;
    if(!pipeString) {
        ptr += 6;
        getinstr(&ptr, str, 0);
        pipeString = str;
    }
    int line, pos, offset;
    while(*ptr == ' ') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &offset);
    --line;
    ptr += offset;
    *sptr = ptr;
    if(checkfile(path + 1)) {
        return;
    }
    createbackup(path + 1);
    addtofile(path + 1, pipeString, getpos(path + 1, line, pos));

}

void cat(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    while(*ptr == ' ') ++ptr;
    ptr += 7;
    getinstr(&ptr, path, 0);
    *sptr = ptr;
    if(checkfile(path + 1)) {
        return;
    }
    copyfile(path + 1, OUTPUT, 0, -1);
}

void removestr(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    int t, size, line, pos, left, right;
    while(*ptr == ' ') ++ptr;
    ptr += 7;
    getinstr(&ptr, path, 0);
    while(*ptr == ' ') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &t);
    --line;
    ptr += t;
    while(*ptr == ' ') ++ptr;
    sscanf(ptr, "-size %d%n", &size, &t);
    ptr += t;
    while(*ptr == ' ') ++ptr;
    *sptr = ptr + 2;
    if(checkfile(path + 1)) {
        return;
    }
    if(*(ptr + 1) == 'f') {
        left = getpos(path + 1, line, pos);
        right = left + size;
    } else {
        right = getpos(path + 1, line, pos);
        left = right - size;
    }
    createbackup(path + 1);
    removefromfile(path + 1, left, right);
}

void undo(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    while(*ptr == ' ') ++ptr;
    ptr += 7;
    getinstr(&ptr, path, 0);
    *sptr = ptr;
    if(checkfile(path + 1)) {
        return;
    }
    restorebackup(path + 1);
}

void copy(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    int t, size, line, pos, left, right;
    while(*ptr == ' ') ++ptr;
    ptr += 7;
    getinstr(&ptr, path, 0);
    while(*ptr == ' ') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &t);
    --line;
    ptr += t;
    while(*ptr == ' ') ++ptr;
    sscanf(ptr, "-size %d%n", &size, &t);
    ptr += t;
    while(*ptr == ' ') ++ptr;
    *sptr = ptr + 2;
    if(checkfile(path + 1)) {
        return;
    }
    if(*(ptr + 1) == 'f') {
        left = getpos(path + 1, line, pos);
        right = left + size;
    } else {
        right = getpos(path + 1, line, pos);
        left = right - size;
    }
    clearfile(CLIPBOARD);
    copyfile(path + 1, CLIPBOARD, left, right);
}

void cut(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    int t, size, line, pos, left, right;
    while(*ptr == ' ') ++ptr;
    ptr += 7;
    getinstr(&ptr, path, 0);
    while(*ptr == ' ') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &t);
    --line;
    ptr += t;
    while(*ptr == ' ') ++ptr;
    sscanf(ptr, "-size %d%n", &size, &t);
    ptr += t;
    while(*ptr == ' ') ++ptr;
    *sptr = ptr + 2;
    if(checkfile(path + 1)) {
        return;
    }
    if(*(ptr + 1) == 'f') {
        left = getpos(path + 1, line, pos);
        right = left + size;
    } else {
        right = getpos(path + 1, line, pos);
        left = right - size;
    }
    clearfile(CLIPBOARD);
    copyfile(path + 1, CLIPBOARD, left, right);
    createbackup(path + 1);
    removefromfile(path + 1, left, right);
}

void paste(char** sptr) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE];
    while(*ptr == ' ') ++ptr;
    ptr += 7;
    getinstr(&ptr, path, 0);
    int line, pos, offset;
    while(*ptr == ' ') ++ptr;
    sscanf(ptr, "--pos %d:%d%n", &line, &pos, &offset);
    --line;
    ptr += offset;
    *sptr = ptr;
    if(checkfile(path + 1)) {
        return;
    }
    createbackup(path + 1);
    faddtofile(path + 1, CLIPBOARD, getpos(path + 1, line, pos));
}

void find(char** sptr, char* pipeString) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE], str[MAX_STRING_SIZE];
    struct wildcard* wcards = NULL;
    int at = 1, bw = 0, cnt = 0, er = 0;
    while(*ptr == ' ') ++ptr;
    if(!pipeString) {
        ptr += 6;
        wcards = getinstr(&ptr, str, 1);
        pipeString = str;
    }
    while(*ptr == ' ') ++ptr;
    ptr += 7;
    getinstr(&ptr, path, 0);
    while(*ptr == ' ') ++ptr;
    int k;
    if(*ptr == '-') {
        if(*(ptr + 1) == 'c') {
            cnt = 1;
            k = 6;
            ++er;
        } else if(*(ptr + 2) == 't') {
            sscanf(ptr, "-at %d%n", &at, &k);
            ++er;
        } else if(*(ptr + 1) == 'a') {
            ++er;
            at = 0;
            k = 4;
        } else if(*(ptr + 1) == 'b') {
            k = 7;
            bw = 1;
        }
        ptr += k;
    }
    while(*ptr == ' ') ++ptr;
    if(*ptr == '-') {
        if(*(ptr + 1) == 'c') {
            cnt = 1;
            k = 6;
            ++er;
        } else if(*(ptr + 2) == 't') {
            sscanf(ptr, "-at %d%n", &at, &k);
            ++er;
        } else if(*(ptr + 1) == 'a') {
            ++er;
            at = 0;
            k = 4;
        } else if(*(ptr + 1) == 'b') {
            k = 7;
            bw = 1;
        }
        ptr += k;
    }
    *sptr = ptr;
    if(checkfile(path + 1)) {
        return;
    }
    if(er > 1 || (cnt && bw)) {
        reterr("Invalid Arguments!");
        return;
    }
    k = 0;
    struct match* ans = findinfile(path + 1, pipeString, wcards, 1);
    FILE* file = fopen(OUTPUT, "w");
    while(ans->left != -1) {
        k++;
        if(!cnt) {
            if(at == k) {
                if(bw) fprintf(file, "%d", ans->word);
                else fprintf(file, "%d", ans->left);
            } else if(!at) {
                if(k > 1) fprintf(file, ", ");
                if(bw) fprintf(file, "%d", ans->word);
                else fprintf(file, "%d", ans->left);
            }
        }
        struct match* tmp = ans->next;
        free(ans);
        ans = tmp;
    }
    if(cnt) fprintf(file, "%d", k);
    else if(!k || k < at) {
        fprintf(file, "-1");
    }
    fprintf(file, "\n");
    fclose(file);
}

int replace(char** sptr) {
    int komak = -1;
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE], str1[MAX_STRING_SIZE], str2[MAX_STRING_SIZE];
    strcpy(path, SLOPEN_FILE);
    struct wildcard* wcards = NULL;
    int at = 1, bw = 0, cnt = 0, er = 0;
    while(*ptr == ' ') ++ptr;
    ptr += 7;
    wcards = getinstr(&ptr, str1, 1);
    while(*ptr == ' ') ++ptr;
    ptr += 7;
    getinstr(&ptr, str2, 0);
    while(*ptr == ' ') ++ptr;
    if(*ptr == '-' && *(ptr + 1) == '-') {
        ptr += 7;
        komak = -2;
        getinstr(&ptr, path, 0);
    }
    while(*ptr == ' ') ++ptr;
    int k;
    if(*ptr == '-') {
        if(*(ptr + 2) == 't') {
            sscanf(ptr, "-at %d%n", &at, &k);
            ++er;
        } else if(*(ptr + 1) == 'a') {
            ++er;
            at = 0;
            k = 4;
        }
        ptr += k;
    }
    while(*ptr == ' ') ++ptr;
    if(*ptr == '-') {
        if(*(ptr + 2) == 't') {
            sscanf(ptr, "-at %d%n", &at, &k);
            ++er;
        } else if(*(ptr + 1) == 'a') {
            ++er;
            at = 0;
            k = 4;
        }
        ptr += k;
    }
    *sptr = ptr;
    if(checkfile(path + 1)) {
        return -1;
    }
    if(er > 1 || (cnt && bw)) {
        reterr("Invalid Arguments!");
        return -1;
    }
    k = 0;
    int ls = 0;
    struct match* ans = findinfile(path + 1, str1, wcards, 1);
    if(ans->left != -1) createbackup(path + 1);
    clearfile(TEMP_PATH);
    while(ans->left != -1) {
        k++;
        if(at == k || !at) {
            if(komak == -1) {
                komak = ans->left;
            }
            copyfile(path + 1, TEMP_PATH, ls, ans->left);
            ls = ans->right;
            strtofile(str2, TEMP_PATH);
        }
        struct match* tmp = ans->next;
        free(ans);
        ans = tmp;
    }
    copyfile(path + 1, TEMP_PATH, ls, -1);
    clearfile(path + 1);
    copyfile(TEMP_PATH, path + 1, 0, -1);
    return komak;
}

void grep(char** sptr, char* pipeString) {
    char* ptr = *sptr;
    char path[FILE_PATH_SIZE], str[MAX_STRING_SIZE];
    struct wildcard* wcards = NULL;
    int lf = 0, cn = 0;
    while(*ptr == ' ') ++ptr;
    if(*(ptr + 1) == 'l') {
        lf = 1;
        ptr += 2;
    } else if(*(ptr + 1) == 'c') {
        cn = 1;
        ptr += 2;
    }
    if(!pipeString){
        while(*ptr == ' ') ++ptr;
        ptr += 6;
        wcards = getinstr(&ptr, str, 1);
        pipeString = str;
    }
    while(*ptr == ' ') ++ptr;
    ptr += 8;
    int k = 0;
    FILE* out = fopen(OUTPUT, "w");
    FILE* tpf = fopen(TEMP_PATH, "w");
    while(*ptr != ' ' && *ptr != '\n' && *ptr != '\0' && *ptr != '=') {
        getinstr(&ptr, path, 0);
        if(checkfile(path + 1)) continue;
        struct match* ans = findinfile(path + 1, pipeString, wcards, 0);
        int fm = 0, mk = 0, i = 0;
        FILE* file = fopen(path + 1, "r");
        char c = fgetc(file);
        while(c != EOF) {
            while(ans->left != -1 && ans->right <= i) {
                struct match* tmp = ans->next;
                free(ans);
                ans = tmp;
            }
            if(ans->left != -1 && ans->left <= i) mk = 1;
            fputc(c, tpf);
            if(c == '\n') {
                k += mk;
                fm += mk;
                if(mk && lf + cn == 0) {
                    fprintf(out, "%s: ", path);
                    fclose(out);
                    fclose(tpf);
                    copyfile(TEMP_PATH, OUTPUT, 0, -1);
                    out = fopen(OUTPUT, "a");
                }
                tpf = fopen(TEMP_PATH, "w");
                mk = 0;
            }
            c = fgetc(file);
            ++i;
        }
        fm += mk;
        k += mk;
        if(mk && lf + cn == 0) {
            fprintf(out, "%s: ", path);
            fclose(out);
            fclose(tpf);
            copyfile(TEMP_PATH, OUTPUT, 0, -1);
            out = fopen(OUTPUT, "a");
            fputc('\n', out);
        }
        tpf = fopen(TEMP_PATH, "w");
        fclose(file);
        if(lf && fm) {
            fprintf(out, "%s\n", path);
        }
    }
    if(cn) {
        fprintf(out, "%d\n", k);
    }
    fclose(tpf);
    fclose(out);
    *sptr = ptr;
}

void tree(char** sptr) {
    char *ptr = *sptr;
    int x, k;
    sscanf(ptr, "%d%n", &x, &k);
    ptr += k;
    *sptr = ptr;
    if(x < 0){
        reterr("Wrong Depth!");
        return;
    }
    prtree(BASE, x, 0);
}

void autoindent(char** sptr, int wtf) {
    char path[FILE_PATH_SIZE];
    if(wtf) {
        strcpy(path, "/");
        strcat(path, OPEN_FILE);
    } else{
        char *ptr = *sptr;
        getinstr(&ptr, path, 0);
        *sptr = ptr;
    }
    if(checkfile(path + 1)) {
        return;
    }
    createbackup(path + 1);
    FILE* file = fopen(path + 1, "r"), *tmp = fopen(TEMP_PATH, "w");
    char c;
    int t = 0, sp = 0, cr = 0, ll = 0, lr = 0;
    while((c = fgetc(file)) != EOF) {
        if(c == ' ') {
            if(cr) sp++;
        } else if(c == '\n') {
            cr = sp = 0;
            if(!ll && !lr) fputc(c, tmp);
            ll = lr = 0;
        } else if(c == '{') {
            if(!cr) {
                for(int i = 0; i < t * TABSIZE; i++) fputc(' ', tmp);
            } else fputc(' ', tmp);
            fputs("{\n",tmp);
            sp = cr = lr = 0;
            ll = 1;
            ++t;
        } else if(c == '}') {
            --t;
            if(cr) fputc('\n', tmp);
            for(int i = 0; i < t * TABSIZE; i++) fputc(' ', tmp);
            fputs("}\n", tmp);
            cr = sp = ll = 0;
            lr = 1;
        } else {
            lr = ll = 0;
            if(cr) {
                for(int i = 0; i < sp; i++) fputc(' ', tmp);
                sp = 0;
            } else {
                for(int i = 0; i < t * TABSIZE; i++) fputc(' ', tmp);
                cr = 1;
            }
            fputc(c, tmp);
        }
    }
    fclose(file);
    fclose(tmp);
    clearfile(path + 1);
    copyfile(TEMP_PATH, path + 1, 0, -1);
}

void compare(char** sptr) {
    char* ptr = *sptr, path1[FILE_PATH_SIZE], path2[FILE_PATH_SIZE], str1[MAX_STRING_SIZE], str2[MAX_STRING_SIZE],
                        wtf1[MAX_STRING_SIZE], wtf2[MAX_STRING_SIZE];
    getinstr(&ptr, path1, 0);
    getinstr(&ptr, path2, 0);
    *sptr = ptr;
    if(checkfile(path1 + 1) || checkfile(path2 + 1)) {
        return;
    }
    FILE *file1 = fopen(path1 + 1, "r"), *file2 = fopen(path2 + 1, "r"), *out = fopen(OUTPUT, "w");
    int ln = 0, kn, L = 0, R = 0;
    while(fgets(str1, MAX_STRING_SIZE, file1) && fgets(str2, MAX_STRING_SIZE, file2)) {
        if(str1[strlen(str1) - 1] != '\n') strcpy(str1 + strlen(str1), "\n");
        if(str2[strlen(str2) - 1] != '\n') strcpy(str2 + strlen(str2), "\n");
        int dif = 0, wn = 0, k = 0;
        char *w1 = str1, *w2 = str2;
        while(1) {
            ++wn;
            while(*w1 == ' ') ++w1;
            while(*w2 == ' ') ++w2;
            if(*w1 == '\n' || *w2 == '\n'){
                if(*w1 != '\n' || *w2 != '\n') dif = 2;
                break;
            }
            while(*w1 != ' ' && *w2 != ' ' && *w1 != '\n' && *w2 != '\n') {
                if(*w1 != *w2) {
                    k = wn;
                }
                ++w1;
                ++w2;
            }
            while(*w1 != ' ' && *w1 != '\n') k = wn, ++w1;
            while(*w2 != ' ' && *w2 != '\n') k = wn, ++w2;
            if(k == wn) ++dif;

        }
        ln++;
        if(!dif) continue;
        fprintf(out, "============ #%d ============\n", ln);
        if(dif == 1) {
            int lp = 1;
            wn = 0;
            w1 = str1;
            w2 = str2;
            while(*w1 != '\n') {
                if(*w1 == ' ') {
                    if(!lp) {
                        if(wn == k) fputs("<<", out);
                    }
                    lp = 1;
                } else {
                    if(lp) {
                        ++wn;
                        if(wn == k) fputs(">>", out);
                    }
                    lp = 0;
                }
                fputc(*w1, out);
                ++w1;
            }
            if(wn == k) fputs("<<", out);
            wn = 0;
            lp = 1;
            fputc('\n', out);
            while(*w2 != '\n') {
                if(*w2 == ' ') {
                    if(!lp) {
                        if(wn == k) fputs("<<", out);
                    }
                    lp = 1;
                } else {
                    if(lp) {
                        ++wn;
                        if(wn == k) fputs(">>", out);
                    }
                    lp = 0;
                }
                fputc(*w2, out);
                ++w2;
            }
            if(wn == k) fputs("<<", out);
            fputc('\n', out);
        } else {
            fputs(str1, out);
            fputs(str2, out);
        }
    }
    kn = ln;
    freopen(path1 + 1, "r", file1);
    freopen(path2 + 1, "r", file2);
    for(int i = 0; i < ln; i++) {
        fgets(str1, MAX_STRING_SIZE, file1);
        fgets(str2, MAX_STRING_SIZE, file2);
    }
    FILE* tmp = fopen(TEMP_PATH, "w");
    while(fgets(str1, MAX_STRING_SIZE, file1)) {
        L = 1;
        kn++;
        if(str1[strlen(str1) - 1] != '\n') strcpy(str1 + strlen(str1), "\n");
        fputs(str1, tmp);
    }
    while(fgets(str2, MAX_STRING_SIZE, file2)) {
        R = 1;
        kn++;
        if(str2[strlen(str2) - 1] != '\n') strcpy(str2 + strlen(str2), "\n");
        fputs(str2, tmp);
    }
    fclose(tmp);
    if(L) {
        fprintf(out, "<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n", ln + 1, kn);
        fclose(out);
        copyfile(TEMP_PATH, OUTPUT, 0, -1);
    } else if(R) {
        fprintf(out, ">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", ln + 1, kn);
        fclose(out);
        copyfile(TEMP_PATH, OUTPUT, 0, -1);
    } else {
        fclose(out);
    }
}