
#include "stdafx.h"
#include "SimpleImage.h"
#include "..\wallet\util\h\define.h"
#include <locale.h>
wchar_t * to_string(char* m_str);
CSimpleImage::CSimpleImage()
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);


}

CSimpleImage::~CSimpleImage()
{
	GdiplusShutdown(m_gdiplusToken);
}

//Ȯ���ڿ� ���� �̹��� ���¸� ���Ѵ�
CString CSimpleImage::GetImageType(CString strImageName)
{
	strImageName.MakeReverse();
	int nPos = strImageName.Find(_T("."));
	if (nPos < 0)
		return SIMPLE_BMP;

	CString strExtension = strImageName.Left(nPos).MakeReverse().MakeLower();

	if (strExtension == _T("bmp"))
		return SIMPLE_BMP;
	else if (strExtension == _T("jpg"))
		return SIMPLE_JPG;
	else if (strExtension == _T("png"))
		return SIMPLE_PNG;
	else if (strExtension == _T("tif"))
		return SIMPLE_TIF;
	else
		return SIMPLE_BMP;
}

//��������� �̹��� ������ ������ �����Ͽ� ���Ϸ� ����
BOOL CSimpleImage::SaveImageData(const BYTE* pImageData, const int nDataLen, LPCTSTR pszOutputFileName)
{
	char ImageData_str[] = ImageData;
	char QR_File_Extension_str[] = QR_File_Extension;
	char QR_File_Name_Temp_str[] = QR_File_Name_Temp;
	CComPtr<IStream> pStream = NULL;
	CComPtr<IStorage> pIStorage = NULL;
	ULONG uWrite = 0;
	HRESULT hr;
	Status stat = Ok;
	///////////////////////////////////////////////////////////////////////
	//1. �����͸� �����ϱ� ���� stream ���� ����
	//������� ����
	hr = StgCreateDocfile(
		NULL,
		STGM_READWRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
		0,
		&pIStorage);

	if (FAILED(hr))
		return NULL;

	
	//stream ����
	hr = pIStorage->CreateStream(
		to_string(ImageData_str), //stream �̸�(�ʼ�����)
		STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
		0,
		0,
		&pStream);

	if (FAILED(hr))
		return NULL;
	///////////////////////////////////////////////////////////////////////
	//2.
	hr = pStream->Write(pImageData, nDataLen, &uWrite);//Byte�� ��Ʈ�� �����͸� stream�� ����
	if (FAILED(hr))
		return FALSE;
	///////////////////////////////////////////////////////////////////////
	//3.
	Gdiplus::Image gImage(pStream);

	CString strFormat = GetImageType(QR_File_Name_Temp_str);

	CLSID ClsId;


	
	GetEncoderClsid(QR_File_Extension_str, &ClsId);
	




	if (strFormat == SIMPLE_JPG)
	{
		//jpg�� ��� ������ ����ȴ�
		EncoderParameters encoderParameters;
		ULONG             quality = 100;//0 ~ 100 ���� ��, ���⼭�� Default�� 75�� �ش�

		encoderParameters.Count = 1;
		encoderParameters.Parameter[0].Guid = EncoderQuality;
		encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
		encoderParameters.Parameter[0].NumberOfValues = 1;
		encoderParameters.Parameter[0].Value = &quality;
		stat = gImage.Save(to_string(QR_File_Name_Temp_str), &ClsId, &encoderParameters);

	}
	else
		stat = gImage.Save(to_string(QR_File_Name_Temp_str), &ClsId);//���Ϸ� ����
																 ///////////////////////////////////////////////////////////////////////
	if (stat == Ok)
		return TRUE;
	else
		return FALSE;
}

//format�� "image/jpeg", "image/bmp", "image/gif", "image/tiff", "image/png" �� 1��
int CSimpleImage::GetEncoderClsid( char* format/*IN*/, CLSID* pClsid/*OUT*/)
{
	UINT  num = 0;          // �̹��� ���ڴ��� ����
	UINT  size = 0;         // �̹��� ���ڴ� �迭�� ����Ʈ ũ�� 

	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);

	if (size == 0)
		return -1;  //����

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

	if (pImageCodecInfo == NULL)
		return -1;  //����

	GetImageEncoders(num, size, pImageCodecInfo);//�̹��� ���ڴ� ������ ����

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, to_string(format)) == 0)//����ڰ� �䱸�� �׷��� ���¿� ��ġ�ϸ�
		{
			*pClsid = pImageCodecInfo[j].Clsid;//�ڵ����̵� ����
			free(pImageCodecInfo);
			return j;  //����
		}
	}

	free(pImageCodecInfo);//����
	return -1;  //����
}
wchar_t * to_string(char* m_str)
{

	//USES_CONVERSION;
	//
	//wchar_t wTemp[256] = L"\0";
	//MultiByteToWideChar(CP_ACP, 0, m_str, m_str.GetLength(), wTemp, 256);







	//wchar_t wstr[256] = {};
	//
	//setlocale(LC_ALL, "korean");
	//if (mblen(m_str, strlen(m_str)) == 2)
	//{
	//	mbstowcs(wstr, (char*)((LPCSTR)m_str), strlen(m_str));
	//
	//}
	//else
	//{
	//	int nLen = m_str.GetLength() + 1;
	//	mbstowcs(wstr, (char*)((LPCSTR)m_str), nLen);
	//}
	LPWSTR lpszW = new WCHAR[MAX_PATH];
	int nLen = MultiByteToWideChar(CP_ACP, 0, m_str, -1, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, m_str, -1, lpszW, nLen);


	return lpszW;
}
