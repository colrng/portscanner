import socket
import termcolor  # 터미널에 컬러를 추가하기 위한 모듈

# 대상과 포트를 스캔하는 함수
def scan(target, ports):
    print('\n' + ' Starting Scan For ' + str(target))  # 대상을 스캔한다는 메시지 출력
    for port in range(1, ports):  # 지정된 포트 범위 내에서 반복
        scan_port(target, port)  # 각 포트에 대해 스캔 수행

# 특정 포트를 스캔하는 함수
def scan_port(ipaddress, port):
    try:
        sock = socket.socket()  # 소켓 생성
        sock.connect((ipaddress, port))  # 소켓을 대상의 지정된 포트로 연결
        print("[+] Port Opened " + str(port))  # 해당 포트가 열렸다는 메시지 출력
        sock.close()  # 소켓 연결 종료
    except:
        pass

# 사용자로부터 대상과 포트 수 입력 받기
targets = input("[*] Enter Targets To Scan (쉼표로 분리): ")  # 스캔할 대상 입력 받기
ports = int(input("[*] Enter How Many Ports You Want To Scan: "))  # 스캔할 포트 수 입력 받기

# 대상이 여러 개인 경우
if ',' in targets:
    print(termcolor.colored(("[*] Scanning Multiple Targets"), 'green'))  # 여러 대상을 스캔 중이라는 메시지 출력
    for ip_addr in targets.split(','):  # 입력된 대상들을 쉼표를 기준으로 분리하여 반복
        scan(ip_addr.strip(' '), ports)  # 각 대상에 대해 스캔 수행
# 대상이 하나인 경우
else:
    scan(targets, ports)