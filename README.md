# COSE214

# 알고리즘

---
## Assignment 1 : Brute force Convex hull problem

```
brute force한 접근방법을 이용하여 convex hull 문제를 해결하기 위하여 (Lec. 3)
주어진 소스코드(bruteforce_convex_hull.c)를 완성한다.
프로그램은 주어진 점(point)의 수만큼 랜덤(randomly)하게 생성된 점들에 대한 convex hull을 구성하는 선분(line segment)들을 찾고,
이를 시각화하기 위해 R 프로그램용 스크립트를 생성한다.
1. 완성된 소스코드의 컴파일은 다음과 같이 한다.
gcc bruteforce_convex_hull.c
(컴파일이 성공한 경우 a.out 이라는 이름의 실행파일이 생성됨)
2. 프로그램 실행 방법은 다음과 같다.
$ ./실행파일 n  > 스크립트명
(n은 양의 정수여야 함)
예) $ ./a.out 10 > convex.R
3. 스크립트 파일은 다음과 같이 실행 권한을 주어야 함
$ chmod +x 스크립트명
4. 스크립트의 실행은 다음과 같이 한다.
$ ./스크립트명
스크립트는 convex_hull을 시각화한 convex.png을 생성한다.
스크립트를 실행하기 위해서는 R 프로그램이 설치되어야 함
설치 확인은 명령행에서 R을 입력하면 됨 (설치되어 있지 않은 경우 화면에 제시된 명령에 따라 패키지를 설치해야 함)
$ sudo apt install r-base-core
5. 폴더를 확인하여 생성된 convex.png를 이미지 뷰어로 열어서 결과를 확인해야 한다.

- 첨부된 실행파일(bruteforce_convex_hull), 스크립트 파일(convex.R), 이미지 파일(convex.png)을 참고할 것.
- valgrind를 통해 메모리 누수(memory leak) 현상을 제거해야 함

- 제출물 : 소스코드(bruteforce_convex_hull.c) (파일명을 변경하지 말 것!)
```
--- 
## Assignment 2 : Divide-and-conquer Convex hull problem

```
Assignment 1의 convex hull 문제를 divide-and-conquer 기법으로 해결하기 위하여 주어진 소스코드(efficient_convex_hull.c)를 완성한다.
- valgrind를 통해 메모리 누수(memory leak) 현상을 제거해야 함
- 기타 사항은 Assignment 1과 동일함

- 제출물 : 소스코드(efficient_convex_hull.c) (파일명을 변경하지 말 것!)
```
--- 
## Assignment 3 : River crossing puzzle problem

```
Lecture 6의 River crossing puzzle 문제를 해결하기 위하여 주어진 소스코드(pwgc.c)를 완성한다.
프로그램의 결과는 pwgc.net을 생성하고, 초기상태로부터 목적상태로의 모든 가능한 경로에 대한 탐색 과정을 화면에 출력해야 한다.
  - 깊이우선(depth-first) 탐색을 사용
  - 현재 상태에서 다음 상태로의 전이는 다음의 순서로 적용해야 함 (농부의 이동, 농부와 늑대의 이동, 농부와 염소의 이동, 농부와 양배추의 이동)
  - pwgc.net은 Pajek 프로그램에서 사용하는 그래프(네트워크) 파일 형식임
  - Pajek 프로그램 페이지 (http://mrvar.fdv.uni-lj.si/pajek/)

- 제출물 : 소스코드(pwgc.c) (파일명을 변경하지 말 것!)
```
--- 
## Assignment 4 : Minimum edit distance / alignment

```
두 문자열(string)의 최소편집거리(minimum edit distance)를 구하고, 그 때의 문자간 정렬(alignment) 결과를 출력하는 프로그램을 구현하기 위해 주어진 소스코드(editdistance.c)를 완성한다.
- 최소편집거리일 때의 문자간 정렬 결과가 둘 이상일 경우, 모든 가능한 정렬 결과를 화면에 출력해야 한다.
- 연산자의 출력이나 순회는 다음의 순서로 적용해야 함 (S(교체) 또는 M(일치), I(삽입), D(삭제), T(전위)) 
프로그램 실행 방법은 다음과 같다.
키보드 입력시
./editdistance 
파일 입력시
./editdistance < 입력파일명 [> 출력파일명]
예) ./editdistance < misspell.txt > result.txt

제출물 : 소스코드(editdistance.c) (파일명을 변경하지 말 것!)
```
--- 
## Assignment 5 : Huffman code implementation

```
허프만 코드에 대한 인코더(encoder)과 디코더(decoder) 프로그램을 작성한다.
인코더는 입력된 텍스트 파일의 문자(alphabet) 빈도에 기반하여 허프만 트리와 허프만 코드를 생성하고, 
허프만 코드를 이용하여 텍스트 파일을 인코딩한다.
디코더는 인코딩된 파일을 허프만 트리를 이용하여 복원한다.
컴파일은 아래와 같이 하면 된다.
$ make
프로그램 실행 방법은 다음과 같다.
$ ./huffman_encoder 입력텍스트파일 인코딩파일
 
예) $ ./huffman_encoder text.txt text.enc
$ ./huffman_decoder 인코딩파일 출력텍스트파일
예) $ ./huffman_decoder text.enc text.dec
- valgrind를 통해 메모리 누수(memory leak) 현상을 제거해야 함

- 제출물 : 모든 소스코드(huffman_encoder.c, huffman_decoder.c, huffman.c, huffman.h, node.h, heap.c, heap.h) 및 Makefile (파일명을 변경하지 말 것!)
```
--- 
