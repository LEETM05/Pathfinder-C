#include "Navigation.h"

typedef struct {
    char str_tmp[1024];
} D;

D arr[MAX_CITY + 1];

int main() {
    FILE* pFile = NULL;
    int k, i = 0;
    char x[SIZE], choose[SIZE];
    char* token;
    initialize(data);

    pFile = fopen("NAVIGATION.csv", "r");

    if (pFile != NULL) {
        while (!feof(pFile)) {
            if (i > MAX_CITY)
                break;
            fgets(arr[i].str_tmp, 1024, pFile);
            token = strtok(arr[i].str_tmp, ",");
            separate_field(&arr[i].str_tmp, token, &data[i]);
            i++;
        }
    }
    else {
        printf("fail\n");
        return 1; // 파일 열기 실패 시 종료
    }

    while (true) {
        print_map(data);    // 맵을 출력한다.
        printf("출발 도시를 입력하세요. : ");
        char Start_City[SIZE];
        scanf("%s", Start_City);
        int start_Code = name_matching(Start_City);

        printf("도착 도시를 입력하세요. : ");
        char End_City[SIZE];
        scanf("%s", End_City);
        int end_Code = name_matching(End_City);
        printf("\n");

        option(start_Code, end_Code);

        // 최소비용 = 1로 리턴, 최소시간 = 2로 리턴, 추천경로 = 3 리턴 
        printf("최소비용 or 최소시간 or 추천경로 : ");
        scanf("%s", &choose);
        int choose_num = name_matching(choose);


        dijkstara(start_Code, choose_num, data);

        printf("%s -> %s  %d원 %d분\n", data[start_Code + 1].name, data[end_Code + 1].name, Cost[end_Code], _time[end_Code]);
        path_node(start_Code, end_Code);
        printf("\n");
        Expect_Time(end_Code);

        printf("\n계속하시겠습니까 ? (Yes or No) ");
        char choice[SIZE];
        scanf("%s", choice);
        if (strcmp(choice, "No") == 0 || strcmp(choice, "no") == 0)
            break;
        else
            system("cls");
    }
    fclose(pFile);

    return 0;
}