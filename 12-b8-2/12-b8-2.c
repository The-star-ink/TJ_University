//2551746 大数据 陈珈铭
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

/* 检查文件是 Windows 格式、Linux 格式还是无法识别 */
int check_format(const char *path) {
  FILE *fp = fopen(path, "rb");
  int c, prev = 0;
  int any_lf = 0, has_crlf = 0, has_lone_lf = 0;

  if (fp == NULL) {
    printf("文件打开失败\n");
    return -1;
  }

  while ((c = fgetc(fp)) != EOF) {
    if (c == '\n') { /* 0x0A */
      any_lf = 1;
      if (prev == '\r') /* 0x0D */
        has_crlf = 1;
      else
        has_lone_lf = 1;
    }
    prev = c;
  }
  fclose(fp);

  if (!any_lf || (has_crlf && has_lone_lf)) {
    printf("文件格式无法识别\n");
    return 0;
  }
  if (has_lone_lf) {
    printf("Linux格式\n");
    return 0;
  }
  printf("Windows格式\n");
  return 0;
}

/* 将 Windows 格式转换为 Linux 格式（0D 0A -> 0A） */
int convert_wtol(const char *inpath, const char *outpath) {
  FILE *fin = fopen(inpath, "rb");
  FILE *fout = NULL;
  int c, prev = 0;
  int any_lf = 0, has_lone_lf = 0;
  int prev_cr = 0;

  if (fin == NULL) {
    printf("文件打开失败\n");
    return -1;
  }

  /* 先判断格式：必须是 Windows 格式 */
  while ((c = fgetc(fin)) != EOF) {
    if (c == '\n') {
      any_lf = 1;
      if (prev != '\r')
        has_lone_lf = 1;
    }
    prev = c;
  }
  fclose(fin);

  if (!any_lf || has_lone_lf) {
    printf("文件格式无法识别\n");
    return 0;
  }

  /* 再转换：删除紧邻 0A 前面的 0D */
  fin = fopen(inpath, "rb");
  fout = fopen(outpath, "wb");
  if (fin == NULL || fout == NULL) {
    if (fin != NULL)
      fclose(fin);
    if (fout != NULL)
      fclose(fout);
    printf("文件打开失败\n");
    return -1;
  }

  while ((c = fgetc(fin)) != EOF) {
    if (prev_cr) {
      if (c == '\n')
        fputc('\n', fout);
      else {
        fputc('\r', fout);
        fputc(c, fout);
      }
      prev_cr = 0;
    } else if (c == '\r') {
      prev_cr = 1;
    } else {
      fputc(c, fout);
    }
  }
  if (prev_cr)
    fputc('\r', fout);

  fclose(fin);
  fclose(fout);
  return 0;
}

/* 将 Linux 格式转换为 Windows 格式（0A -> 0D 0A） */
int convert_ltow(const char *inpath, const char *outpath) {
  FILE *fin = fopen(inpath, "rb");
  FILE *fout = NULL;
  int c, prev = 0;
  int any_lf = 0, has_crlf = 0;

  if (fin == NULL) {
    printf("文件打开失败\n");
    return -1;
  }

  /* 先判断格式：必须是 Linux 格式 */
  while ((c = fgetc(fin)) != EOF) {
    if (c == '\n') {
      any_lf = 1;
      if (prev == '\r')
        has_crlf = 1;
    }
    prev = c;
  }
  fclose(fin);

  if (!any_lf || has_crlf) {
    printf("文件格式无法识别\n");
    return 0;
  }

  /* 再转换：每个 0A 前插入 0D */
  fin = fopen(inpath, "rb");
  fout = fopen(outpath, "wb");
  if (fin == NULL || fout == NULL) {
    if (fin != NULL)
      fclose(fin);
    if (fout != NULL)
      fclose(fout);
    printf("文件打开失败\n");
    return -1;
  }

  while ((c = fgetc(fin)) != EOF) {
    if (c == '\n') {
      fputc('\r', fout);
      fputc('\n', fout);
    } else {
      fputc(c, fout);
    }
  }

  fclose(fin);
  fclose(fout);
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc >= 3 && strcmp(argv[1], "--check") == 0) {
    return check_format(argv[2]);
  }
  if (argc >= 5 && strcmp(argv[1], "--convert") == 0) {
    if (strcmp(argv[2], "wtol") == 0)
      return convert_wtol(argv[3], argv[4]);
    if (strcmp(argv[2], "ltow") == 0)
      return convert_ltow(argv[3], argv[4]);
  }
  return -1;
}
