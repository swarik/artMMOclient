#include <iostream>
#include <string>
#include <unistd.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#define API_KEY "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VybmFtZSI6InN3YXJnYSIsInBhc3N3b3JkX2NoYW5nZWQiOiIifQ.0PPcseUf-gJtqJrzA1oWAwsqm4J-0A61SdSl_yGCShA"

using namespace std;
using json = nlohmann::json;

//
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s);
CURLcode GEquip(string Item_code, string Item_slot);
CURLcode GRecycle(string Item_code, int Item_num);
CURLcode GUnEquip(string Item_slot);
CURLcode GCraft(string Item_code);
CURLcode GMove(int x,int y);
CURLcode GFight();
CURLcode GGet();
CURLcode GStatus_not_parse();
string GStatus(string); // распарсенный статус перса
int GStatus(string, string); // получение количества предметов инвентаря
string readBuffer="";
string pers = "Swarik";

int main() {
	bool Exit = false;
	string command = "";

	int X, Y;

	while (!Exit){

	cout << endl << pers << ":>> ";
	cin >> command;


		if (command == "pers") {                 // СМЕНИТЬ ПЕРСОНАЖ - pers
			cout << "Кем играть ? Сейчас - " << pers << " = ";
			cin >> ::pers;
		}
		if (command == "go") {                   // ПЕРЕМЕЩЕНИЕ - go
			cout << "X Y = ";
			cin >> X >> Y;
			GMove(X,Y);
		}

		if (command == "equip") {                // ОДЕТЬ ЭКИПИРОВКУ - equip
			string Slot = "";
			string Equip_code = "";
			cout << "Equip code  Slot = ";
			cin >> Equip_code >> Slot;
			GEquip(Equip_code, Slot);
		}

		if (command == "unequip") {              // СНЯТЬ ЭКИПИРОВКУ - unequip (weapon,shield,helmet,body_armor,leg_armor,boots,ring1,ring2,amulet,artifact1,artifact2,artifact3,consumable1,consumable2)
			string Slot = "";
			cout << "Slot = ";
			cin >> Slot;
			GUnEquip(Slot);
		}

		if (command == "get") {                  // СБОР РЕСУРСОВ - get
			int Sleep_time = 1;
			int L_cont = 0;
			cout << "How_many_resources_need  Wait_time = ";
			cin >> L_cont >> Sleep_time;
			while (L_cont){
				GGet();
				L_cont--;
				cout << "Осталось собрать " << L_cont << endl;
				sleep(Sleep_time);
			}
		}

		if (command == "fight") {                // СРАЖЕНИЕ - fight
			int Sleep_time = 1;
	        int L_cont = 0;
			cout << "How_many_monsters  Wait_time = ";
			cin >> L_cont >> Sleep_time;
	        while (L_cont){
	        	GFight();
	        	L_cont--;
	        	cout << "Осталось добить " << L_cont << endl;
	        	sleep(Sleep_time);
	        }
		}
		if (command == "craft") {                // КРАФТ - craft
			string Code = "";
			int Sleep_time = 1;
			int L_cont = 0;
			cout << "Что_крафтить  Сколько  Wait_time = ";
			cin >> Code >> L_cont >> Sleep_time;
			while (L_cont){
				GCraft(Code);
				L_cont--;
				cout << "Осталось скрафтить " << L_cont << endl;
				sleep(Sleep_time);
			}
		}
		if (command == "recycle") {              // УТИЛИЗАЦИЯ - recycle
			string Equip_code = "";
			int Equip_num = 0;
			cout << "Util equip code  Slot = ";
			cin >> Equip_code >> Equip_num;
			GRecycle(Equip_code, Equip_num);
			sleep(6);
		}
		if (command == "status"){                // СТАТУСЫ ПЕРСОНАЖА - status, inv - инвентарь.
			string What_code = "";
			cout << "Какой параметр? = ";
			cin >> What_code;
			if (What_code == "inv"){
				cout << "Что именно? = ";
				string s1;
				cin >> s1;
				cout << s1 << ": " << GStatus("inventory", s1);
			}
			else cout << What_code << ": " << GStatus(What_code);
		}
		if (command == "scenario"){              // ПРОКАЧКА СКИЛЛА - scenario
			int t1;

		while(true){
			//////////////// L1 ////////////

			GMove(2,0);
			sleep(12);

			t1=90;
			while(t1){
				GGet();
				sleep(25);
				t1--;
			}

			GMove(1,5);
			sleep(32);

			t1=15;
			while(t1){
				GCraft("copper");
				sleep(6);
				t1--;
			}

			///////////// L2 ////////////

			GMove(2,0);
			sleep(32);

			t1=90;
			while(t1){
				GGet();
				sleep(25);
				t1--;
			}

			GMove(1,5);
			sleep(32);

			t1=15;
			while(t1){
				GCraft("copper");
				sleep(6);
				t1--;
			}

			////////////// NEXT ///////////

			GMove(3,1);
			sleep(32);

			t1=10;
			while(t1){
				GCraft("copper_boots");
				sleep(6);
				t1--;
			}

			sleep(120);

			GRecycle("copper_boots", 10);
			sleep(60);

			t1=5;
			while(t1){
				GCraft("copper_boots");
				sleep(6);
				t1--;
			}

			sleep(120);

			GRecycle("copper_boots", 5);
			sleep(60);



		}   // бесконечный цикл)
			/////////////////////////////////
		}
		if (command == "clear") cout << "\033[2J\033[1;1H";           // ОЧИСТКА КОНСОЛИ - clear
	    if (command == "exit" || command == "quit") Exit = true;      // ВЫХОД - exit
	    if (command == "help") cout << "pers go get fight equip unequip craft recycle status(inv) scenario clear exit" << endl;  // СПРАВКА - help
	    if (command == "t"){



	    	cout << GStatus("inventory", "egg");




	    }
	}
	return 0;

}

//***************
//***************
CURLcode GMove(int x, int y){

	// Функция передвижения.
	CURL *hnd = curl_easy_init();

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(hnd, CURLOPT_URL, ("https://api.artifactsmmo.com/my/" + pers + "/action/move").c_str());

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, ("Authorization: Bearer " + string(API_KEY)).c_str());
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
//*****************************
	string zapr = "{\n  \"x\": " +to_string (x)+ ",\n  \"y\": " +to_string(y)+ "\n}";
//*****************************
	cout << endl << pers << endl;
	sleep(5);


	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, zapr.c_str());

	CURLcode ret = curl_easy_perform(hnd);
	//clear curls)
	curl_easy_cleanup(hnd);
	curl_slist_free_all(headers);
	curl_global_cleanup();

	cout << "\033[2J\033[1;1H";  // очищаем консоль)

	return ret;
}
//***************
//***************
CURLcode GGet(){

	// Функция сбора ресурсов.
	CURL *hnd = curl_easy_init();

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(hnd, CURLOPT_URL, ("https://api.artifactsmmo.com/my/" + pers + "/action/gathering").c_str());

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, ("Authorization: Bearer " + string(API_KEY)).c_str());
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	CURLcode ret = curl_easy_perform(hnd);
	//clear curls)
	curl_easy_cleanup(hnd);
	curl_slist_free_all(headers);
	curl_global_cleanup();

	cout << "\033[2J\033[1;1H";  // очищаем консоль)

	return ret;
}
//***************
//***************
CURLcode GFight(){

	// Функция сбора ресурсов.
	CURL *hnd = curl_easy_init();

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(hnd, CURLOPT_URL, ("https://api.artifactsmmo.com/my/" + pers + "/action/fight").c_str());

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, ("Authorization: Bearer " + string(API_KEY)).c_str());
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	CURLcode ret = curl_easy_perform(hnd);
	//clear curls)
	curl_easy_cleanup(hnd);
	curl_slist_free_all(headers);
	curl_global_cleanup();

	cout << "\033[2J\033[1;1H";  // очищаем консоль)

	return ret;
}
//***************
//***************
CURLcode GUnEquip(string Item_slot){

	// Функция снятия экипировки.

		CURL *hnd = curl_easy_init();

		curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(hnd, CURLOPT_URL, ("https://api.artifactsmmo.com/my/" + pers + "/action/unequip").c_str());

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, ("Authorization: Bearer " + string(API_KEY)).c_str());
		curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	//*****************************
		string zapr = "{\n  \"slot\": \"" + Item_slot + "\"\n}";
	//*****************************
		//cout << endl << zapr << endl;


		curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, zapr.c_str());

		CURLcode ret = curl_easy_perform(hnd);
		//clear curls)
		curl_easy_cleanup(hnd);
		curl_slist_free_all(headers);
		curl_global_cleanup();

		cout << "\033[2J\033[1;1H";  // очищаем консоль)

		return ret;
	}
//***************
//***************
CURLcode GEquip(string Item_code, string Item_slot){

	// Функция снаряжения экипировки в выбранный слот.

		CURL *hnd = curl_easy_init();

		curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(hnd, CURLOPT_URL, ("https://api.artifactsmmo.com/my/" + pers + "/action/equip").c_str());

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, ("Authorization: Bearer " + string(API_KEY)).c_str());
		curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	//*****************************
		string zapr = "{\n  \"code\": \"" + Item_code + "\",\n  \"slot\": \"" + Item_slot + "\"\n}";
	//*****************************
		//cout << endl << zapr << endl;


		curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, zapr.c_str());

		CURLcode ret = curl_easy_perform(hnd);
		//clear curls)
		curl_easy_cleanup(hnd);
		curl_slist_free_all(headers);
		curl_global_cleanup();

		cout << "\033[2J\033[1;1H";  // очищаем консоль)

		return ret;
	}
//***************
//***************
CURLcode GCraft(string Item_code){

	// Функция крафта.

		CURL *hnd = curl_easy_init();

		curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(hnd, CURLOPT_URL, ("https://api.artifactsmmo.com/my/" + pers + "/action/crafting").c_str());

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, ("Authorization: Bearer " + string(API_KEY)).c_str());
		curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	//*****************************
		string zapr = "{\n  \"code\": \"" + Item_code + "\"\n}";
	//*****************************
		//cout << endl << zapr << endl;


		curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, zapr.c_str());

		CURLcode ret = curl_easy_perform(hnd);
		//clear curls)
		curl_easy_cleanup(hnd);
		curl_slist_free_all(headers);
		curl_global_cleanup();

		cout << "\033[2J\033[1;1H";  // очищаем консоль)

		return ret;
	}
//***************
//***************
CURLcode GRecycle(string Item_code, int Item_num){

	// Функция утилизации.

		CURL *hnd = curl_easy_init();

		curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(hnd, CURLOPT_URL, ("https://api.artifactsmmo.com/my/" + pers + "/action/recycling").c_str());

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, ("Authorization: Bearer " + string(API_KEY)).c_str());
		curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	//*****************************
		string zapr = "{\n  \"code\": \"" + Item_code + "\",\n  \"quantity\": \"" + to_string(Item_num) + "\"\n}";
	//*****************************
		//cout << endl << zapr << endl;


		curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, zapr.c_str());

		CURLcode ret = curl_easy_perform(hnd);
		//clear curls)
		curl_easy_cleanup(hnd);
		curl_slist_free_all(headers);
		curl_global_cleanup();

		cout << "\033[2J\033[1;1H";  // очищаем консоль)

		return ret;
	}
//***************
//***************
CURLcode GStatus_not_parse(){

	// Техническая функция получения статуса перса.
	CURL *hnd = curl_easy_init();

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(hnd, CURLOPT_URL, "https://api.artifactsmmo.com/my/characters");

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, ("Authorization: Bearer " + string(API_KEY)).c_str());
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);
	CURLcode ret = curl_easy_perform(hnd);

	curl_easy_cleanup(hnd);
	curl_slist_free_all(headers);
	curl_global_cleanup();
	cout << "\033[2J\033[1;1H";  // очищаем консоль)

	return ret;
}
//***************
//***************
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}
//***************
//***************
string GStatus(string s1){

	// получение статуса персонажа в распарсенном формате json

	GStatus_not_parse();
	json j = json::parse(readBuffer);
	readBuffer.clear();
	int sel_pers;
	if (pers == "Swarik") sel_pers = 0;
	if (pers == "Kuzma") sel_pers = 1;
	return to_string(j["data"][sel_pers][s1]);
}
int GStatus(string s1, string s2){

	// получение количества предметов в инвентаре

	GStatus_not_parse();
	json j = json::parse(readBuffer);
	readBuffer.clear();
	int sel_pers;
	if (pers == "Swarik") sel_pers = 0;
	if (pers == "Kuzma") sel_pers = 1;
	int max_inv_slots = 20;
	int s = 255;
	for(int i = 0; i < max_inv_slots ; i++){
		if ((j["data"][sel_pers][s1][i]["code"]) == s2) {
			s = i;
			break;
		}
	}
	if (s == 255) return 0;
	return j["data"][sel_pers][s1][s]["quantity"];
}
//***************
//***************
