/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
char display[25][80];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int bombnumber = 0;
uint8_t rec_Data[1];
uint8_t rx_index = 0;
uint8_t rx_data;
uint8_t rx_buffer[20];

char stageOfGame[9][19];
char showTable[9][19];
int nowX = 0;
int nowY = 0;
int isChange = 0;
char keyBoard[1];
int isDied = 0;
//					1		2		3		4		5		6		7		8		9		10		11
int rand1[25][2] = {{0, 5}, {0, 8}, {0, 10},{0, 16},{1, 2}, {1, 5}, {1, 8}, {2, 0}, {2, 16},{4, 7}, {4, 13}, {4, 17},{5, 5},  {5, 17}, {6, 6},  {6, 7}, {6, 10}, {6, 11}, {6, 14},{7, 0},  {7, 6}, {8, 1},{8, 3},{8, 15},{8, 18}};
int rand2[19][2] = {{0 ,3}, {1 ,1}, {1 ,10}, {2 ,5}, {2 ,15}, {4 ,5}, {4 ,17}, {5 ,2}, {6 ,1}, {6 ,12}, {7 ,1}, {7 ,7}, {7 ,8}, {7 ,10}, {8 ,5}, {8 ,6}, {8 ,12}, {8 ,17}};
int rand3[22][2] = {{0 ,2}, {0 ,15}, {1 ,13}, {1 ,14}, {1 ,15}, {2 ,2}, {4 ,8}, {4 ,9}, {5 ,13}, {6 ,9}, {6 ,11}, {7 ,5}, {7 ,6}, {7 ,9}, {7 ,12}, {8 ,0}, {8 ,2}, {8 ,3}, {8 ,14}, {8 ,16}};
int rand4[18][2] = {{0 ,1}, {0 ,3}, {0 ,14}, {1 ,1}, {1 ,5}, {1 ,10}, {2 ,2}, {2 ,7}, {3 ,8}, {4 ,14}, {5 ,11}, {6 ,1}, {6 ,12}, {7 ,5}, {7 ,15}, {8 ,5}, {8 ,13}};
int rand5[20][2] = {{0 ,17}, {1 ,0}, {1 ,4}, {1 ,7}, {1 ,10}, {2 ,3}, {2 ,7}, {2 ,8}, {2 ,9}, {2 ,10}, {3 ,7}, {3 ,18}, {4 ,6}, {4 ,12}, {5 ,8}, {6 ,10}, {7 ,16}, {8 ,13}, {8 ,15}};
int randSize[] = {25, 18, 20, 17, 19};
int bombCount = 0;
int winCounter = 0;
int randCount = 0;
int isWinValue = 0;
int time = 0;
int isLose = 0;
int isPress[9][19];
int isStop = 1;
int isStart = 0; //0 is not start, 1 is already start
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		if(isStop == 0){
			time++;
		}
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
		HAL_UART_Receive_IT(&huart2, keyBoard, sizeof(keyBoard));
		if(isDied == 1 || isWinValue == 1){
			isDied = 0;
			isWinValue = 0;
			for(int i=0; i<9; i++){
				for(int j=0; j<19; j++){
					showTable[i][j] = '.';
				}
			}
		}
		clearDisplay();
		layout();
		if(keyBoard[0] == 'w'){
			if(nowY>0) nowY--;
			else if (nowY == 0) nowY = 8;
		}
		else if(keyBoard[0] == 'a'){
			if(nowX>0) nowX--;
			else if(nowX == 0) nowX = 18;
		}
		else if(keyBoard[0] == 's'){
			if(nowY<8) nowY++;
			else if(nowY == 8) nowY = 0;
		}
		else if(keyBoard[0] == 'd'){
			if(nowX<18) nowX++;
			else if(nowX == 18) nowX = 0;
		}
		else if(keyBoard[0] == 'l'){
			if(isStart == 0){
				isStart = 1;
				time = 0;
				isStop = 0;
			}
			if(stageOfGame[nowY][nowX] == '*'){
				randCount++;
				if(randCount == 5) randCount = 0;
				isDied = 1;
				isLose = 1;
			}
			else if(stageOfGame[nowY][nowX] == ' '){
				showTable[nowY][nowX] = ' ';
				bfs(nowX, nowY);
			}
			else{
				showTable[nowY][nowX] = stageOfGame[nowY][nowX];
			}
		}
		else if(keyBoard[0] == 'p'){
			if(isStart == 0){
				isStart = 1;
				time = 0;
				isStop = 0;
			}
			showTable[nowY][nowX] = 'F';
			if(stageOfGame[nowY][nowX] == '*' && isPress[nowY][nowX] == 0){
				bombCount--;
				isPress[nowY][nowX] = 1;
			}
		}
		show();


	}

}
int isWin(){
	char c[] = {'1','2','3','4','5','6','7','8'};
	int count = 0;
	for(int i=0; i<9; i++){
		for(int j=0; j<19; j++){
			if(showTable[i][j] == ' ' || isIn(c, showTable[i][j], 8) == 1){
				count++;
			}
		}
	}
	if(count == (171 - randSize[randCount])) return 1;
	else return 0;
}
int isIn(char a[], int x, int length){
	for(int i=0; i<length; i++){
		if(a[i] == x) return 1;
	}
	return 0;
}
void bfs(int x, int y){
	int queue[200][2];
	int pointerLeft = 0;
	int pointerRight = 1;
	int move[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
	queue[0][0] = y;
	queue[0][1] = x;
	while(pointerLeft<pointerRight){
		int myY = queue[pointerLeft][0];
		int myX = queue[pointerLeft][1];
		pointerLeft++;
		for(int i=0; i<8; i++){
			int tmpX = move[i][0] + myX;
			int tmpY = move[i][1] + myY;
			if(tmpX>=0 && tmpY>=0 && tmpX<19 && tmpY<9 && stageOfGame[tmpY][tmpX] != '*' && showTable[tmpY][tmpX] == '.'){
				if(stageOfGame[tmpY][tmpX] == ' '){
					queue[pointerRight][0] = tmpY;
					queue[pointerRight][1] = tmpX;
					pointerRight++;
				}
				showTable[tmpY][tmpX] = stageOfGame[tmpY][tmpX];
			}
		}
	}
}
void setBomb(){
	for(int i=0; i<9; i++){
		for(int j=0; j<19; j++){
			isPress[i][j] = 0;
		}
	}
	bombCount = randSize[randCount];
	for(int i=0; i<9; i++){
		for(int j=0; j<19; j++) stageOfGame[i][j] = ' ';
	}
	if(randCount % 5 == 0){
		for(int i=0; i<randSize[randCount]; i++){
				stageOfGame[rand1[i][0]][rand1[i][1]] = '*';
		}
	}
	else if(randCount % 5 == 1){
		for(int i=0; i<randSize[randCount]; i++){
			stageOfGame[rand2[i][0]][rand2[i][1]] = '*';
		}
	}
	else if(randCount % 5 == 2){
		for(int i=0; i<randSize[randCount]; i++){
			stageOfGame[rand3[i][0]][rand3[i][1]] = '*';
		}
	}
	else if(randCount % 5 == 3){
		for(int i=0; i<randSize[randCount]; i++){
			stageOfGame[rand4[i][0]][rand4[i][1]] = '*';
		}
	}
	else if(randCount % 5 == 4){
		for(int i=0; i<randSize[randCount]; i++){
			stageOfGame[rand5[i][0]][rand5[i][1]] = '*';
		}
	}
	for(int i=0; i<9; i++){
		for(int j=0; j<19; j++){
			int m = 0;
			if(stageOfGame[i][j] != '*'){
				if(i-1>=0 && stageOfGame[i-1][j] == '*') m++;
				if(i+1<9 && stageOfGame[i+1][j] == '*') m++;
				if(j-1>=0 && stageOfGame[i][j-1] == '*') m++;
				if(j+1<19 && stageOfGame[i][j+1] == '*') m++;
				if(i-1>=0 && j-1>=0 && stageOfGame[i-1][j-1] == '*') m++;
				if(i-1>=0 && j+1<19 && stageOfGame[i-1][j+1] == '*') m++;
				if(i+1<9 && j-1>=0 && stageOfGame[i+1][j-1] == '*') m++;
				if(i+1<9 && j+1<19 && stageOfGame[i+1][j+1] == '*') m++;
				if(m>0){
					char c = m + '0';
					stageOfGame[i][j] = c;
				}
				else{
					stageOfGame[i][j] = ' ';
				}
			}
		}
	}
}
void clearDisplay(){
	char clear[9] = {'A', 27, '[', '2', 'J', 27, '[', 'H'};
	for(int i=0; i<8; i++){
		char tmp[1];
		tmp[0] = clear[i];
		HAL_UART_Transmit(&huart2, tmp, sizeof(tmp), 100000);
	}

}
void startLayout(){
	setBomb();
	for(int i=0; i<25; i++){
		for(int j=0; j<80; j++){
			display[i][j] = ' ';
		}
	}
	char *ch0 = ",-.-.o          ,---.                              ";
	char *ch1 = "| | |.,---.,---.`---.. . .,---.,---.,---.,---.,---.";
	char *ch2 = "| | |||   ||---'    || | ||---'|---'|   ||---'|    ";
	char *ch3 = "` ' '``   '`---'`---'`-'-'`---'`---'|---'`---'`    ";
	char *ch4 = "                                    |              ";



	for(int i=0; i<51; i++){
		display[8][i+15] = ch0[i];
		display[9][i+15] = ch1[i];
		display[10][i+15] = ch2[i];
		display[11][i+15] = ch3[i];
		display[12][i+15] = ch4[i];
	}
	for(int i=0; i<9; i++){
		for(int j=0; j<19; j++){
			showTable[i][j] = '.';
		}
	}
	char *ch8 = "Press any key to continue!!!";
	for(int i=0; i<28; i++){
		display[23][i+26] = ch8[i];
	}
	for(int i=0; i<25; i++){
		for(int j=0; j<80; j++){
			char tmp[1];
			tmp[0] = display[i][j];
			HAL_UART_Transmit(&huart2, tmp, sizeof(tmp), 100000);
		}
	}

}
void youLose(){
	setBomb();
	nowX = 0;
	nowY = 0;
	for(int i=0; i<4; i++){
		for(int j=0; j<50; j++){
			display[i][j] = ' ';
		}
	}
	for(int i=0; i<80; i++){
		display[0][i] = ' ';
	}
	char *ch0 =  " __ __            __                ";
	char *ch1 =  "|  |  |___ _ _   |  |   ___ ___ ___ ";
	char *ch2 =  "|_   _| . | | |  |  |__| . |_ -| -_|";
	char *ch3 =  "  |_| |___|___|  |_____|___|___|___|";




	for(int i=0; i<36; i++){
			display[0][i+5] = ch0[i];
			display[1][i+5] = ch1[i];
			display[2][i+5] = ch2[i];
			display[3][i+5] = ch3[i];
	}
	char *ch8 = "Press enter to continue!!!";
	for(int i=0; i<28; i++){
		display[23][i+26] = ch8[i];
	}



}
void youWin(){
	setBomb();
	nowX = 0;
	nowY = 0;
	for(int i=0; i<4; i++){
		for(int j=0; j<50; j++){
			display[i][j] = ' ';
		}
	}
	for(int i=0; i<80; i++){
		display[0][i] = ' ';
	}
	char *ch0 = " __ __            _ _ _ _     ";
	char *ch1 = "|  |  |___ _ _   | | | |_|___ ";
	char *ch2 = "|_   _| . | | |  | | | | |   |";
	char *ch3 = "  |_| |___|___|  |_____|_|_|_|";


	for(int i=0; i<30; i++){
				display[0][i+5] = ch0[i];
				display[1][i+5] = ch1[i];
				display[2][i+5] = ch2[i];
				display[3][i+5] = ch3[i];

		}
		char *ch8 = "Press any key to continue!!!";
		for(int i=0; i<28; i++){
			display[20][i+26] = ch8[i];
		}





}
void layout(){
	for(int i=0; i<25; i++){
		for(int j=0; j<80; j++){
			display[i][j] = ' ';
		}
	}
	for(int i=0; i<80; i++){
		display[0][i] = ' ';
		display[79][i] = ' ';
	}
	for(int i=0; i<25; i++){
		display[i][0] = ' ';
		display[i][24] = ' ';
	}
	for(int i=1; i<79; i++){
		display[0][i] = '-';
		display[24][i] = '-';
	}
	for(int i=0; i<23; i++){
		for(int j=0; j<80; j++){
			if(i>=4 && i%2==0 && j<78 && j>=1){
				if(j%4==1) display[i][j] = '|';
				else display[i][j] = '-';
			}


			else if(i>=5 && j%4==1 && j<79 && i%2==1) display[i][j] = '|';
		}
	}
	display[2][36] = '(';
	display[2][37] = '^';
	display[2][38] = '_';
	display[2][39] = '^';
	display[2][40] = ')';
	char number[13] = {'B', 'o', 'm', 'b', ' ', 'n', 'u', 'm', 'b', 'e', 'r', ' ',':'};
	for(int i=0; i<13; i++){
		display[2][i+6] = number[i];
	}

	/*char location[] = "Now you are at (";
	for(int i=0; i<16; i++){
		display[2][i+50] = location[i];
	}*/


}
void show(){
	if(isDied == 0){
		for(int i=0; i<9; i++){
				for(int j=0; j<19; j++){
					display[mapY(i)][mapX(j)] = showTable[i][j];
				}
			}
	}
	else{
		for(int i=0; i<9; i++){
			for(int j=0; j<19; j++){
				display[mapY(i)][mapX(j)] = stageOfGame[i][j];
			}
		}
	}
	if(bombCount < 10){
		display[2][20] = '0';
		char cppp = bombCount + '0';
		display[2][21] = cppp;
	}
	else{
		int p = bombCount / 10;
		char pp = p+ '0';
		int q = bombCount % 10;
		char qq = q+ '0';
		display[2][20] = pp;
		display[2][21] = qq;
	}
	int y = mapY(nowY);
	int x = mapX(nowX);
	display[y][x-2] = '$';
	display[y][x+2] = '$';
	display[y-1][x] = '$';
	display[y+1][x] = '$';
	int tmpTime = time;

	/*int tmpX = nowX + 1;
	int tmpY = nowY + 1;
	if(tmpX<10){
		display[2][66] = '0';
		char c = tmpX + '0';
		display[2][67] = c;
	}
	else{
		display[2][66] = '1';
		tmpX -= 10;
		char c = tmpX + '0';
		display[2][67] = c;
	}
	display[2][68] = ',';
	display[2][69] = ' ';
	if(tmpY<10){
		display[2][70] = '0';
		char c = tmpY + '0';
		display[2][71] = c;
	}
	else{
		display[2][70] = '1';
		tmpY -= 10;
		char c = tmpY + '0';
		display[2][71] = c;
	}

	display[2][72] = ')';*/
	for(int i=0; i<9; i++){
			for(int j=0; j<19; j++){
				if(showTable[i][j] == '.'){
					display[mapY(i)][mapX(j)] = '.';
					display[mapY(i)][mapX(j)+1] = '.';
					display[mapY(i)][mapX(j)-1] = '.';
				}
			}
		}
	if(isDied == 1 && isLose == 1){
		time = 0;
		isStart = 0;
		isStop = 1;
		for(int i=0; i<9; i++){
			for(int j=0; j<19; j++){
				if(stageOfGame[i][j] == '*'){
					display[mapY(i)][mapX(j)] = '*';
					display[mapY(i)][mapX(j)-1] = ' ';
					display[mapY(i)][mapX(j)+1] = ' ';
				}
			}
		}
		youLose();
	}
	else if(isWin() == 1){
		time = 0;
		isStart = 0;
		isStop = 1;
		randCount++;
		if(randCount == 5) randCount = 0;
		isWinValue = 1;
		for(int i=0; i<9; i++){
			for(int j=0; j<19; j++){
				if(stageOfGame[i][j] == '*'){
					display[mapY(i)][mapX(j)] = '*';
					display[mapY(i)][mapX(j)-1] = ' ';
					display[mapY(i)][mapX(j)+1] = ' ';
				}
			}
		}
		youWin();
	}
	char *yourTime = "Your time is : ";
	for(int i=0; i<15; i++){
		display[2][50+i] =  yourTime[i];
	}
	if(tmpTime<1000){
			display[2][65] = '0';
		}
		else{
			int t = tmpTime / 1000;
			tmpTime %= 1000;
			char c = t + '0';
			display[2][65] = c;
		}
		if(tmpTime<100){
			display[2][66] = '0';
		}
		else{
			int t = tmpTime / 100;
			tmpTime %= 100;
			char c = t + '0';
			display[2][66] = c;
		}
		if(tmpTime<10){
			display[2][67] = '0';
		}
		else{
			int t = tmpTime / 10;
			tmpTime %= 10;
			char c = t + '0';
			display[2][67] = c;
		}
		char pppp = tmpTime + '0';
		display[2][68] = pppp;
	for(int i=0; i<25; i++){
		for(int j=0; j<80; j++){
			char tmp[1];
			tmp[0] = display[i][j];
			HAL_UART_Transmit(&huart2, tmp, sizeof(tmp), 100000);
		}
	}

}
int mapX(int x){
	return 4*x+3;
}
int mapY(int y){
	return 2*y+5;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
  HAL_TIM_Base_Start_IT(&htim2);
  clearDisplay();
  startLayout();
  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 35999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 2000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
