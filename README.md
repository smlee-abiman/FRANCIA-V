
# 인재 PC에서 도커 컨테이너 빌드 할때 사용하는 코드

6. cd /home/tietech/buildsdk
7. . ./environment-setup
8. cd build
9. qmake ../../../tietech/AEK/pendant/Pendant.pro
10. make -j16
---

의존성이 남아있는 건 **빌드 디렉토리를 클린하지 않아서**입니다. 도커 환경에서 아래 순서로 실행하세요:

```bash
cd /home/tietech/buildsdk
. ./environment-setup

# 빌드 디렉토리 완전 초기화
cd build
rm -rf *

# qmake + 빌드
qmake ../../../tietech/AEK/pendant/Pendant.pro
make -j16
```

`rm -rf *`으로 기존 Makefile과 `.d` 의존성 파일을 모두 삭제한 후 qmake를 새로 실행하면 `loading_3.png` 의존성이 완전히 사라집니다.