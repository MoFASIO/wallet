
Mikks HD Wallet 초안

1. BIP39 니모닉 대신 유저가 원하는 문장. (조사, 특수기호 포함) 사용.
  1) NFKD(UTF-8)
  1) 최소 글자수 제한 (빈 공백 포함)

2. 유저 Password (UTF-8 NFKD)
  1) 도형, 색상, 음식, 장소 등에서 암호 사용을 권장
  2) 개인별 Password 입력 (최소 글자수 제한)

3. PBKDF2 함수를 이용하여 Seed 생성.
  1) Password = "유저가 원하는 문장" + "개인별 Password"
  -> personalSentence + personalPassword
  2) Salt = "랜덤바이트(2바이트)" + "개인별 Password"
  -> rng + personalPassword

4. 생성된 Seed로 HD Wallet의 MasterNode를 구성.
  1) secp256k1, secp256r1 지원

5. 생성된 MasterNode로 Bip32, Bip44의 제안에 따른 Child 키 파생.

웹 spec

page 구성

  1.지갑 생성
    1) 문장과 비밀번호를 작성
    2) 랜덤바이트를 제공하고 그 외의 MasterNode의 정보를 제공
       (Seed, PrivateKey, ChainCode, PublicKey, extendedKey(xprv and xpub))
  2.지갑 조회
    1) 문장과 비밀번호 랜덤바이트를 입력
    2) 지갑의 MasterNode 정보를 제공
    ('1.지갑 생성'과 동일한 함수 실행, 랜덤바이트의 초기 값을 받는다는 차이일 뿐)
  3.키 파생
    1) 확장 키와 derivePath를 입력받는다.
    2) childKey의 Node 정보를 제공
  4.주소 생성
    1) 퍼블릭 키를 입력받음



랜덤바이트를 하나 생성하고, 암호로 입력받은 값을 xor해서 나온 1바이트의 값을 랜덤바이트와 xor해서 나온 값 1바이트 둘을 이어서 2바이트의 랜덤바이트를 생성한다.
