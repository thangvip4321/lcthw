set -e
sudo devpkg -S
sudo devpkg -F https://github.com/grpc/grpc.git
sudo devpkg -B https://github.com/grpc/grpc.git