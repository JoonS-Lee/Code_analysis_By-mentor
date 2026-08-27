# MCU-SPEC-Generator (hd-project21)

MCU 베어메탈 C 소스코드를 역설계(Reverse Engineering)하여 기능 사양서(Draft)를 자동 생성하는 도구입니다.
단일 HTML 파일(`docs/index.html`)로 동작하며, CSS/JS 전부 문서 내에 포함되어 있고 외부 라이브러리·CDN에 의존하지 않습니다.

## 배포

- GitHub Pages: `docs/` 폴더 소스, `main` 브랜치
- 리포는 **private** — 소스코드가 브라우저 밖으로 나가지 않지만, 대외비 원본 자료 유출 방지를 위해 리포 자체도 비공개로 유지합니다.
- `aebonlee.github.io/hd-project21/` 로 접속

## 사용 방법

1. **Phase 1** — 분석할 `.c`/`.h`/`.txt` 파일을 업로드하거나 코드를 붙여넣습니다. (CP949/UTF-8 자동 인식)
2. **Phase 2** — 함수·분류·제어 흐름·CAN/신호 참조가 자동 추출된 결과를 확인합니다.
3. **Phase 3** — 표지 정보(관리번호·Rev·담당/검토/승인 등)를 입력하고 사양서 Draft를 미리 봅니다.
4. **Phase 4** — 사양서 항목과 소스 근거의 교차 검증 결과를 확인한 뒤 Word(.docx) 파일을 다운로드합니다.

## 핵심 기술 포인트

- **전처리기 조건부 컴파일 해석**: `#if/#ifdef/#ifndef/#elif/#else/#endif`를 실제로 해석해 활성 코드만 함수로 추출합니다.
  리터럴 `0`/`1`, `defined()` 등은 평가하고, 정의되지 않은 매크로(`TEST_MODE` 등)는 기본적으로 비활성(`#else`) 경로를 채택하며 "확인 필요" 항목으로 표시합니다(§6.1 원칙).
- **CAN/J1939 신호 추출**: `구조체SPN.필드`, `RX_/TX_/CAN_..._<PGN>` 형태의 식별자를 스캔해 함수별 CAN 통신 표를 자동 구성합니다.
- **제어 흐름 구조화**: if/else/switch/return을 트리로 파싱해 화면에는 SVG 다이어그램, Word 문서에는 들여쓰기+색상 기호(◆/◇/■) 구조화 텍스트로 각각 렌더링합니다.
- **.docx 자체 생성**: 외부 라이브러리(docx.js 등) 없이 CRC32 + ZIP(STORE) writer를 직접 구현해 OOXML을 조립합니다.
  LibreOffice(headless)로 실제 소스 5,098줄을 넣어 생성한 문서가 텍스트/PDF로 정상 변환됨을 확인했습니다.

## 디렉터리 구조

```
hd-project21/
├── docs/index.html      # 배포되는 앱 (Pages가 서빙하는 유일한 경로)
├── reference/            # 원본 자료 보관용 — Pages에 노출되지 않음
│   └── Alogrithm.c       # 실제 분석 대상 원본 소스 (대외비 — 절대 docs/ 로 이동 금지)
└── README.md
```

⚠️ **`reference/` 폴더는 절대 `docs/` 밑으로 옮기지 마십시오.** GitHub Pages가 서빙하는 경로에 두면
리포가 private이어도 배포된 사이트를 통해 파일이 그대로 공개 다운로드됩니다(§3.8 원칙).
