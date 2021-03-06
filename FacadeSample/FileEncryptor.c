
#include <stdio.h>
#include "FileEncryptor.h"

//属性、方法声明

struct FileEncryptor_Fld
{
    apr_pool_t *m_pPool;
};

static char *Encrypt(FileEncryptor *pInst, const char *pPlainText, apr_pool_t *pPool)
{
    puts("数据加密，转换为密文。");

    size_t szLen = strlen(pPlainText);
    char *pEncrypted = apr_palloc(pPool, szLen - 1/* '\0' */);
    *(pEncrypted + szLen) = '\0';
    int nKey = 10;
    int nIndex = 0;
    for (size_t i = 0; i < szLen; i++)
    {
        int nTmp = (int)*(pPlainText + i);
        //小写字母移位
        if (*(pPlainText + i) >= 'a' && *(pPlainText + i) <= 'z')
        {
            nTmp += nKey % 26;
            if (nTmp > 122)
            {
                nTmp -= 26;
            }
            if (nTmp < 97)
            {
                nTmp += 26;
            }
        }
        //大写字母移位
        if (*(pPlainText + i) >= 'A' && *(pPlainText + i) <= 'Z')
        {
            nTmp += nKey % 26;
            if (nTmp > 90)
            {
                nTmp -= 26;
            }
            if (nTmp < 65)
            {
                nTmp += 26;
            }
        }

        *(pEncrypted + nIndex++) = (char)nTmp;
    }
    fprintf(stdout, "加密后: %s", pEncrypted);

    return pEncrypted;
}

FileEncryptor * FileEncryptor_New(apr_pool_t * pSupPool)
{
    apr_pool_t *pPool;
    apr_pool_create(&pPool, pSupPool);

	FileEncryptor *pInst = apr_palloc(pPool, sizeof(FileEncryptor));

    pInst->pFld = apr_palloc(pPool, sizeof(FileEncryptor_Fld));
    pInst->pFld->m_pPool = pPool;

    pInst->Encrypt = Encrypt;

    return pInst;
}

void FileEncryptor_Free(FileEncryptor ** ppInst)
{
    apr_pool_destroy((*ppInst)->pFld->m_pPool);
    *ppInst = NULL;
}
