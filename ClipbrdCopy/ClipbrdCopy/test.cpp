#include "CopyImgFromClpbrd.h"

int main(void)
{
    int nYes = 0;

    // copy text from clipboard
    nYes = AfxMessageBox( _T( "处理标准剪切板文本？" ), MB_YESNO | MB_ICONINFORMATION );
    if (IDYES == nYes) {
        CopyTxtFromClpbrd();
    }

    // copy image from clipboard
    nYes = AfxMessageBox( _T( "处理标准剪切板图像？" ), MB_YESNO | MB_ICONINFORMATION );
    if (IDYES == nYes) {
        CopyImgFromClpbrd();
    }

    // define clipboard format
    nYes = AfxMessageBox( _T( "处理自定义格式剪切板？" ), MB_YESNO | MB_ICONINFORMATION );
    if (IDYES == nYes) {
        //ManualFormatClipboard();
        DefineFormatClpbrd();
    }

    // copy data from ole clipboard
    nYes = AfxMessageBox( _T( "处理OLE剪切板？" ), MB_YESNO | MB_ICONINFORMATION );
    if (IDYES == nYes) {
        CopyImgFromOLEClpbrd();
    }

    return 0;
}