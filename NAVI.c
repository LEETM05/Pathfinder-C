#include "Navigation.h"

void initialize(MAP data[]) {

    for (int i = 1; i < MAX_CITY; i++) {
        data[i].row = -1;
        data[i].col = -1;
        *data[i].name = " ";
        real_virtual_price[i - 1] = INF;
    }
    real_virtual_price[MAX_CITY - 1] = INF;

}

void separate_field(char* s, char* token, MAP* data) {
    int i = 0, j = 0, t = 0;

    while (token != NULL) {
        if (i == 0)
            strcpy(data->name, token);
        else if (i == 1)
            data->row = atoi(token); // 행에 해당하는 부분을 저장
        else if (i == 2)
            data->col = atoi(token); // 열에 해당하는 부분을 저장
        else if (i > 2 && i % 2 == 1)
            data->cost[j++] = atoi(token); // 비용을 저장
        else
            data->time[t++] = atoi(token); // 걸리는 시간을 저장
        i++;
        token = strtok(NULL, ","); // ,를 기준으로 엑셀을 구분한다.

    }
}

int name_matching(char* str) {
    int k = 0;
    if (strcmp(str, "최소비용") == 0)
        return 1;
    else if (strcmp(str, "최소시간") == 0)
        return 2;
    else if (strcmp(str, "추천경로") == 0)
        return 3;

    for (int i = 1; i < MAX_CITY; i++) {
        if (strcmp(str, data[i].name) != 0)
            k++;
        if (strcmp(str, data[i].name) == 0)
            break;
    }
    return k;
}

int getSmallIndex(int choose) {
    int min = INF;
    int index = 0;
    for (int i = 0; i < MAX_CITY; i++) {
        if (choose == 1 && !v[i] && Cost[i] < min) {
            min = Cost[i];
            index = i;
        }
        else if (choose == 2 && !v[i] && _time[i] < min) {
            min = _time[i];
            index = i;
        }
        else if (choose == 3 && !v[i] && real_virtual_price[i] < min) {
            min = real_virtual_price[i];
            index = i;
        }
    }
    return index;
}

void dijkstara(int start, int choose, MAP G[]) {
    for (int i = 0; i < MAX_CITY; i++) {
        Cost[i] = data[start + 1].cost[i];
        _time[i] = data[start + 1].time[i];
        transit[i] = start; // 여기서 transit을 출발 도시로 초기화
        real_virtual_price[i] = Cost[i] + _time[i] * 150;  // 진짜 가상값을 초기화 
        v[i] = false;
    }
    v[start] = true;

    for (int i = 0; i < MAX_CITY - 1; i++) {
        int current = getSmallIndex(choose);
        v[current] = true;
        for (int j = 0; j < MAX_CITY; j++) {
            if (!v[j]) {
                int new_cost = Cost[current] + data[current + 1].cost[j];
                int new_time = _time[current] + data[current + 1].time[j];
                if (new_cost <= Cost[j] && choose == 1) {   // 최소비용 
                    Cost[j] = new_cost;
                    _time[j] = new_time;
                    transit[j] = current; // 최단 경로를 업데이트할 때 경유 도시도 업데이트
                }
                else if (new_time <= _time[j] && choose == 2) {   // 최소시간 
                    _time[j] = new_time;
                    Cost[j] = new_cost;
                    transit[j] = current;
                }
                else if (choose == 3) {     // choose == 3 추천경로 
                    // 가상 가격 계산 (비용 * 1 + 시간 * 150 + 환승 가중치)
                    int virtual_price = new_cost + (new_time * 150);  // 시간과 가격의 비율을 1:150으로 변경
                    if (transit[current] != start) {    // 환승 한다면 가중치 30% 적용
                        virtual_price += virtual_price * 0.30;
                    }
                    // 비교 후 저장
                    if (virtual_price <= real_virtual_price[j]) {
                        real_virtual_price[j] = virtual_price;
                        Cost[j] = new_cost;
                        _time[j] = new_time;
                        transit[j] = current;
                    }
                }
            }
        }
    }
}

void option(int start, int end) {
    char option_name[SIZE];
    for (int i = 1; i <= 3; i++) {
        int current_node = end;
        int count = 1;  // 시작점 
        dijkstara(start, i, data);
        switch (i) {
        case 1:
            strcpy(option_name, "최소비용"); break;
        case 2:
            strcpy(option_name, "최소시간"); break;
        case 3:
            strcpy(option_name, "추천경로"); break;
        }
        while (current_node != start) {   // 현재의 노드가 start_Code 이면 break 
            current_node = transit[current_node];
            count++;
        }
        printf("%d. %s 방법은 %s -> %s\n", i, option_name, data[start + 1].name, data[end + 1].name);
        printf("%d원 %d분\n", Cost[end], _time[end]);
        printf("환승 %d번\n\n", count - 2);    // 출발지역과 도착지역을 보정해줌 
    }
}

void path_node(int start, int end) {
    int path[MAX_CITY];
    int path_length = 0;
    int current_node = end;

    while (current_node != start) {   // 현재의 노드가 start_Code 이면 break;
        path[path_length++] = current_node;
        current_node = transit[current_node];
    }
    path[path_length++] = start;   // 현재의 값을 path 끝값에 넣어줌 
    printf("--------------------------------------\n");
    printf("출발 도시: %s\n", data[start + 1].name);
    printf("도착 도시: %s\n", data[end + 1].name);
    printf("거쳐가는 도시: ");
    for (int i = path_length - 1; i >= 0; i--) {   // 역순으로  출력하여 거쳐가는 장소를 모두 프린트해줌 
        printf("%s ", data[path[i] + 1].name);  // 데이터는 1부터 시작
        if (i >= 1) {
            printf(" -> ");
        }
    }
    printf("\n비용: %d원", Cost[end]);
}

void print_map(MAP data[]) {
    printf("전체화면으로 변경 후 엔터를 눌러주세요. ");
    getchar();
    system("cls");

    // y = 1
    gotoXY(43, 1);
    printf("____");   // 휴전선 인근 (46까지)


    // y = 2
    gotoXY(39, 2);
    printf("__--");   // 휴전선 인근 (42까지)
    gotoXY(47, 2);
    printf("--_");   // 휴전선 강원도 인근(49까지)
    gotoXY(53, 2);
    printf(" ___");   // 휴전선 강원도 인근(56까지)


    // y = 3
    gotoXY(35, 3);
    printf("__/"); // 휴전선 인근 (38까지)
    gotoXY(50, 3);
    printf("\\__/");   // 휴전선 강원도 인근 (52까지)
    gotoXY(57, 3);
    printf("\\");   // 휴전선 강원도 인근


    // y = 4
    gotoXY(21, 4);
    printf("____");   // 강화도 (24까지)
    gotoXY(31, 4);
    printf("____");   // 휴전선 인근 (34까지)
    gotoXY(58, 4);
    printf("|");   // 강원도 인근 


    // y = 5
    gotoXY(20, 5);
    printf("|");   // 강화도 제작 
    gotoXY(25, 5);
    printf("|");   // 강화도 
    gotoXY(30, 5);
    printf("/");
    gotoXY(59, 5);   // 동해안(강원)
    printf("\\");



    // y = 6
    gotoXY(21, 6);
    printf("----");   // 강화도 제작 
    gotoXY(29, 6);
    printf("/");
    gotoXY(60, 6);   // 동해안(강원)
    printf(" \\");


    // y = 7
    gotoXY(28, 7);
    printf("/");
    gotoXY(62, 7);   // 동해안(강원)
    printf(" \\");

    // y = 8
    gotoXY(27, 8);
    printf("|");
    gotoXY(64, 8);   // 동해안(강원)
    printf("\\");

    // y = 9
    gotoXY(27, 9);
    printf("\\");
    gotoXY(65, 9);   // 동해안(강원)
    printf("\\");
    gotoXY(80, 9);   // 울릉도 
    printf("◇");

    // y = 10

    gotoXY(28, 10);
    printf("\\");
    gotoXY(66, 10);   // 동해안(강원)
    printf("\\");


    // y = 11
    gotoXY(29, 11);
    printf("\\");
    gotoXY(21, 11);
    printf("-----");
    gotoXY(67, 11);
    printf("|");
    gotoXY(85, 11);   // 독도 
    printf("◇");

    // y = 12
    gotoXY(31, 12);
    printf("\\");
    gotoXY(26, 12);
    printf("\\");
    gotoXY(20, 12);
    printf("/");
    gotoXY(67, 12);   // 동해안(강원)
    printf(" |");

    // y = 13
    gotoXY(27, 13);
    printf("-----");
    gotoXY(19, 13);
    printf("/");
    gotoXY(67, 13);   // 동해안(강원)
    printf(" |");

    // y = 14
    gotoXY(18, 14);
    printf("/");
    gotoXY(67, 14);   // 동해안(경북)
    printf("|");

    // y = 15
    gotoXY(18, 15);
    printf("|");
    gotoXY(66, 15);
    printf("/");

    // y = 16
    gotoXY(19, 16);
    printf("\\");
    gotoXY(65, 16);   // 동해안(경상)
    printf("/");

    // y = 17
    gotoXY(20, 17);
    printf("\\");
    gotoXY(64, 17);   // 동해안(경상)
    printf("/");

    // y = 18 
    gotoXY(21, 18);
    printf("|");
    gotoXY(63, 18);   // 동해안(경상)
    printf("/");

    // y = 19
    gotoXY(21, 19);
    printf("|");
    gotoXY(62, 19);   // 동해안(경상)
    printf("|");

    // y = 20
    gotoXY(20, 20);
    printf("/");
    gotoXY(62, 20);   // 동해안(경상)
    printf("|");

    // y = 21
    gotoXY(19, 21);
    printf("/");
    gotoXY(64, 21);   // 동해안 (경남)
    printf(" _");
    gotoXY(62, 21);   // 동해안 (경남)
    printf("|");

    // y = 22
    gotoXY(18, 22);
    printf("/");
    gotoXY(66, 22);   // 동해안
    printf("\\");
    gotoXY(63, 22);   // 동해안
    printf("\\/");


    // y = 23
    gotoXY(17, 23);
    printf("/");
    gotoXY(65, 23);
    printf("/");   // 동해안


    // y = 24
    gotoXY(16, 24);
    printf("/");
    gotoXY(64, 24);   // 동해안
    printf("/");

    // y = 25
    gotoXY(15, 25);
    printf("|");
    gotoXY(63, 25);   // 동해안
    printf("/");


    // y = 26
    gotoXY(15, 26);
    printf("|");
    gotoXY(62, 26);   // 동해안 라인 
    printf("/");

    // y = 27
    gotoXY(15, 27);
    printf("|");
    gotoXY(62, 27);   // 동해안 라인 
    printf("/");

    // y = 28
    gotoXY(15, 28);
    printf("/");
    gotoXY(61, 28);   // 동해안 라인 
    printf("|");

    // y = 29
    gotoXY(14, 29);
    printf("|");
    gotoXY(61, 29);   // 동해안 라인
    printf("|");


    // y = 30
    gotoXY(15, 30);
    printf("|");
    gotoXY(57, 30);
    printf("__--");   // 남해안 라인 x = 51까지

    // y = 31
    gotoXY(15, 31);
    printf("|");
    gotoXY(47, 31);
    printf("_____-----");   // 남해안 라인 x = 56까지 

    // y = 32 
    gotoXY(15, 32);
    printf("|");
    gotoXY(37, 32);
    printf("_____-----");   // 남해안 라인 x = 46까지 

    // y = 33
    gotoXY(15, 33);
    printf("|");
    gotoXY(27, 33);
    printf("_____-----");   // 남해안 라인 x = 36까지 

    // y = 34 
    gotoXY(16, 34);
    printf("_____------");

    // y = 38 
    gotoXY(22, 38);
    printf("______");   // x = 27까지 -> 제주도 제작

    // y = 39
    gotoXY(21, 39);
    printf("|");
    gotoXY(28, 39);
    printf("|");

    // y = 40
    gotoXY(20, 40);
    printf("|");
    gotoXY(29, 40);
    printf("|");

    // y = 41
    gotoXY(21, 41);
    printf("|");
    gotoXY(28, 41);
    printf("|");

    // y = 42
    gotoXY(22, 42);
    printf("------");

    for (int i = 1; i < MAX_CITY + 1; i++) {
        gotoXY(data[i].row, data[i].col);
        printf("%s", data[i].name);
    }
    gotoXY(0, 43);  // 출력좌표 리셋
    printf("\n");

}

void gotoXY(int _x, int _y)
{
    COORD pos = { _x, _y };

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Expect_Time(int end) {
    time_t ti = time(NULL);
    struct tm tm = *localtime(&ti);
    int hour = 0, min = 0;
    if (_time[end] >= 60) {
        hour += _time[end] / 60;
        min = _time[end] % 60;
    }
    hour += tm.tm_hour;
    min += tm.tm_min;
    if (min >= 60) {
        hour += min / 60;
        min %= 60;
    }
    if (hour >= 24)
        hour -= 24;

    printf("현재시간 : %d시 %d분\n", tm.tm_hour, tm.tm_min);
    printf("예상도착시간 : %d시 %d분\n", hour, min);
    printf("--------------------------------------\n");
}