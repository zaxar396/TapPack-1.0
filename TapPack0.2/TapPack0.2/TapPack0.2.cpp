#include<iostream>
#include"pcap.h"
#include<fstream>
#pragma comment(lib,"wpcap.lib")
#pragma warning(disable : 4996)

using namespace std; 

void another_callback(u_char *arg, const struct pcap_pkthdr* pkthdr,
	const u_char* packet)
{
	int i = 0;
	static int count = 0;
	char* count_ = nullptr;
	int radix = 10;  //система счисления
	char buffer[20];
	FILE* data;
	data = fopen("B:\\AProg\\Programms\\TapPack0.2\\TapPack0.2\\data.txt", "a");

	char dt[20];
	char pc[] = "Packet Count : ";
	char rps[] = "Recieved Packet Size: ";
	char p[] = "Payload:";
	char t[] = " . ";


	cout << "Packet Count : " << ++count << endl;
	
	count_ =  itoa(count,buffer,radix);
	fwrite(pc, sizeof(char), strlen(pc), data);
	fwrite(&count_, sizeof(char),strlen(count_), data);
	fprintf(data,"\n");
	cout << "Recieved Packet Size: " << pkthdr->len << endl;
	fwrite(rps, sizeof(char), strlen(rps), data);
	fwrite(&pkthdr->len, sizeof(bpf_u_int32), 10, data);
	fprintf(data, "\n");
	cout << "Payload:" << endl;
	fwrite(p, sizeof(char), strlen(p), data);
	fprintf(data, "\n");
	for (i = 0; i<pkthdr->len; i++) {
		if (isprint(packet[i])) {
			//fout << packet[i];
			fwrite(&packet[i], sizeof(u_char), 1, data);
			fprintf(data, "\n");
			cout << packet[i] << endl;
		}
		else
		cout << " . " << packet[i];
		fwrite(t, sizeof(char), 3, data);
		fwrite(&packet[i], sizeof(u_char), 1, data);
		//fout << " . " << packet[i];
		if ((i % 16 == 0 && i != 0) || i == pkthdr->len - 1) {
			cout << endl;
			fprintf(data, "\n");
			//fout << endl;
		}

	}

}

int StrToInt(char *s)
{
	int temp = 0; // число
	int i = 0;
	int sign = 0; // знак числа 0- положительное, 1 - отрицательное
	if (s[i] == '-')
	{
		sign = 1;
		i++;
	}
	while (s[i] >= 0x30 && s[i] <= 0x39)
	{
		temp = temp + (s[i] & 0x0F);
		temp = temp * 10;
		i++;
	}
	temp = temp / 10;
	if (sign == 1)
		temp = -temp;
	return(temp);
}

int main()
{
	//for (int i = 0; i < strlen(argv[1]); i++)
	//{
	//	cout << argv[1][i];
	//}
    
	char *dev, errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle;
	pcap_t *handle1;
	pcap_t *handle2;
	pcap_t *handle3;
	struct bpf_program filter;
	char filter_app[] = "port 80";
	char filter_app1[] = "port 52";
	char filter_app2[] = "port 53";
	char filter_app3[] = "port 54";
	bpf_u_int32 mask;
	bpf_u_int32 net;
	struct pcap_pkthdr header;

	ifstream f;
	char buff[256];

	int number;
	int j;
	/*if (argc > 1)
	{
		//number = StrToInt(argv[1]);
		cmd = argv[1];
	}*/

	char devname[100][100], devs[100][100];
	int count = 0, n;
	pcap_if_t *alldevsp = nullptr, *device;
	cout << "Finding available devices ... ";
	if (pcap_findalldevs(&alldevsp, errbuf))
	{
		cout << "Error finding devices : " << errbuf;
		exit(1);
	}
	
	cout << "Done" << endl;

	cout << "Available Devices are :" << endl;
	for (device = alldevsp; device != NULL; device = device->next)
	{
		cout << count << ". " << device->name << " - " << device->description << endl;
		if (device->name != NULL)
		{
			strcpy_s(devs[count], device->name);
			strcpy_s(devname[count], device->description);
		}
		count++;
	}
/*	if (argc > 1) {
		for (int i = 0; i < count; i++)
		{
			if (strcmp(devname[i], argv[1]) == 0)
			{
				j = i;
			}
			else {
				cout << "Nope ";
			}
		}
	}
	*/
	//dev = pcap_lookupdev(errbuf);
	//cout << dev << endl;

	f.open("B:\\AProg\\Programms\\TapPack0.4\\TapPack0.4\\cmd.txt");
	f.getline(buff, 256);
	for (int i = 0; i < count; i++)
	{
		if (strcmp(devname[i], buff) == 0)
		{
			j = i;
		}
	}


	handle = pcap_open_live(devs[j], BUFSIZ, 1, -1, errbuf);
	pcap_lookupnet(devs[j], &net, &mask, errbuf);
	
	

	pcap_compile(handle, &filter, filter_app, 0, net);
	pcap_setfilter(handle, &filter);
	pcap_loop(handle, -1, another_callback, NULL);

	handle1 = pcap_open_live(devs[j], BUFSIZ, 1, -1, errbuf);
	pcap_compile(handle1, &filter, filter_app1, 0, net);
	pcap_setfilter(handle1, &filter);
	pcap_loop(handle1, -1, another_callback, NULL);

	handle2 = pcap_open_live(devs[j], BUFSIZ, 1, -1, errbuf);
	pcap_compile(handle2, &filter, filter_app2, 0, net);
	pcap_setfilter(handle2, &filter);
	pcap_loop(handle2, -1, another_callback, NULL);

	handle3 = pcap_open_live(devs[j], BUFSIZ, 1, -1, errbuf);
	pcap_compile(handle3, &filter, filter_app3, 0, net);
	pcap_setfilter(handle3, &filter);
	pcap_loop(handle3, -1, another_callback, NULL);


	//const u_char *packet;
	//packet = pcap_next(handle, &header);
	//cout << header.len << endl;

	pcap_close(handle);
	pcap_close(handle1);
	pcap_close(handle2);
	pcap_close(handle3);

	system("pause");
	return(0);
}