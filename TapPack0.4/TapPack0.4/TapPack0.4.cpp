#pragma comment(lib,"wpcap.lib")
#include"pcap.h"
#include <Windows.h>
#include"resource.h"


#pragma warning(disable : 4996) 

CHAR *dev;
CHAR errbuf[PCAP_ERRBUF_SIZE];
pcap_t *handle;
pcap_t *handle1;
pcap_t *handle2;
pcap_t *handle3;
struct bpf_program filter;
char filter_app[] = "port 80";
bpf_u_int32 mask;
bpf_u_int32 net;
struct pcap_pkthdr header;
const u_char *packet;
CHAR devname[100][100], devs[100][100];
int count = 0,countname = 1, n;
pcap_if_t *alldevsp = nullptr, *device;
STARTUPINFO si;
PROCESS_INFORMATION pi;

int Len = 0;
int i_dev;


char* IntToStr(int n)
{
	char s[40], t, *temp;
	int i, k;
	int sign = 0;
	i = 0;
	k = n;
	if (k<0)
	{
		sign = 1;
		k = -k;
	}
	do {
		t = k % 10;
		k = k / 10;
		s[i] = t | 0x30;
		i++;
	} while (k>0);
	if (sign == 1)
	{
		s[i] = '-';
		i++;
	}
	temp = new char[i];
	k = 0;
	i--;
	while (i >= 0) {
		temp[k] = s[i];
		i--; k++;
	}
	temp[k] = '\0';
	return(temp);
}


void finddevices()
{
	if (pcap_findalldevs(&alldevsp, errbuf))
	{	
		exit(1);
	}
	for (device = alldevsp; device != NULL; device = device->next)
	{
		if (device->name != NULL)
		{
			strcpy_s(devs[count], device->name);
			strcpy_s(devname[count], device->description);
		}

		count++;
	}
}






HWND hCombo;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc);
	return 0;

}


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		finddevices();
		hCombo = GetDlgItem(hwnd, IDC_COMBO1);

		
		for (int indx = 0; indx < count; indx++)
		{
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)&devname[indx]);
		}
		
		
		
	}
	break;

	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		     case IDC_COMBO1:
		    {
				if (HIWORD(wParam) == CBN_SELENDOK)
				{
					CHAR cmd[255];
					FILE* f;
					int len;
					f = fopen("cmd.txt", "w");
					int i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
                    SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)cmd);
					len = strlen(cmd);
					fwrite(cmd, sizeof(char), len, f);
					if (!CreateProcess(
						"B:\\AProg\\Programms\\TapPack0.2\\Debug\\TapPack0.2.exe",
						NULL,
						NULL,
						NULL,
						FALSE,
						0,
						NULL,
						NULL,
						&si,
						&pi)) {
						// обработка ошибок
					}
					fclose(f);
				}
				break;
		    }

			 case IDC_BUTTON1:
			 {
				 system("B:\\Games\\Notepad\\Notepad++\\notepad++.exe B:\\AProg\\Programms\\TapPack0.2\\TapPack0.2\\data.txt");
				 break;
			 }
		}

		break;

	//case WM_TIMER:

		//sniff(i_dev);



	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return FALSE;
	}
	return FALSE;
}