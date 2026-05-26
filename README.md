#  ふつうのLinuxプログラミングの写経

## 最後の成果物、httpd2 のコンパイル

```bash
gcc chapter16/httpd2.c -o chapter/httpd2
```

## HTTPサーバ起動

```bash
sudo ./chapter/httpd2  --port=8080 --chroot --user=<USER> --group=<GROUP> "$(pwd)/chapter16/example/www/"
```
