#include "CopyImgFromClpbrd.h"

int main(void)
{
    int nYes = 0;

    // copy text from clipboard
    nYes = AfxMessageBox( _T( "�����׼���а��ı���" ), MB_YESNO | MB_ICONINFORMATION );
    if (IDYES == nYes) {
        CopyTxtFromClpbrd();
    }

    // copy image from clipboard
    nYes = AfxMessageBox( _T( "�����׼���а�ͼ��" ), MB_YESNO | MB_ICONINFORMATION );
    if (IDYES == nYes) {
        CopyImgFromClpbrd();
    }

    // define clipboard format
    nYes = AfxMessageBox( _T( "�����Զ����ʽ���а壿" ), MB_YESNO | MB_ICONINFORMATION );
    if (IDYES == nYes) {
        //ManualFormatClipboard();
        DefineFormatClpbrd();
    }

    // copy data from ole clipboard
    nYes = AfxMessageBox( _T( "����OLE���а壿" ), MB_YESNO | MB_ICONINFORMATION );
    if (IDYES == nYes) {
        CopyImgFromOLEClpbrd();
    }

    return 0;
}