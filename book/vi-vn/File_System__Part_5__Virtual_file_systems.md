## Hệ thống tệp ảo
Các hệ thống POSIX, chẳng hạn như Linux và Mac OSX (dựa trên BSD) bao gồm một số hệ thống tệp ảo được gắn kết (khả dụng) như một phần của hệ thống tệp. Các tệp bên trong các hệ thống tệp ảo này không tồn tại trên đĩa; chúng được kernel tạo động khi một tiến trình yêu cầu danh sách thư mục.
Linux cung cấp 3 hệ thống tệp ảo chính:
```
/dev  - Danh sách các thiết bị vật lý và ảo (ví dụ: card mạng, ổ đĩa CD-ROM, trình tạo số ngẫu nhiên)
/proc - Danh sách các tài nguyên được sử dụng bởi mỗi tiến trình và (theo truyền thống) tập hợp thông tin hệ thống
/sys - Danh sách có tổ chức các thực thể kernel nội bộ
```

Ví dụ: nếu bạn muốn một luồng số 0 liên tục, bạn có thể sử dụng lệnh `cat /dev/zero`.

## Làm cách nào để tìm ra hệ thống tệp nào hiện có sẵn (được gắn kết)?
Sử dụng lệnh `mount`.

Sử dụng `mount` mà không có bất kỳ tùy chọn nào sẽ tạo ra một danh sách (một hệ thống tệp trên mỗi dòng) các hệ thống tệp được gắn kết, bao gồm các hệ thống tệp mạng, ảo và cục bộ (đĩa quay / dựa trên SSD). Dưới đây là đầu ra điển hình của `mount`:

```
$ mount
/dev/mapper/cs241--server_sys-root on / type ext4 (rw)
proc on /proc type proc (rw)
sysfs on /sys type sysfs (rw)
devpts on /dev/pts type devpts (rw,gid=5,mode=620)
tmpfs on /dev/shm type tmpfs (rw,rootcontext="system_u:object_r:tmpfs_t:s0")
/dev/sda1 on /boot type ext3 (rw)
/dev/mapper/cs241--server_sys-srv on /srv type ext4 (rw)
/dev/mapper/cs241--server_sys-tmp on /tmp type ext4 (rw)
/dev/mapper/cs241--server_sys-var on /var type ext4 (rw)rw,bind)
/srv/software/Mathematica-8.0 on /software/Mathematica-8.0 type none (rw,bind)
engr-ews-homes.engr.illinois.edu:/fs1-homes/angrave/linux on /home/angrave type nfs (rw,soft,intr,tcp,noacl,acregmin=30,vers=3,sec=sys,sloppy,addr=128.174.252.102)
```
Lưu ý rằng mỗi dòng bao gồm loại hệ thống tệp, nguồn của hệ thống tệp và điểm gắn kết.
Để giảm đầu ra này, chúng ta có thể chuyển nó vào `grep` và chỉ xem các dòng khớp với biểu thức chính quy. 
```bash
>mount | grep proc  # chỉ xem các dòng chứa 'proc'
proc on /proc type proc (rw)
none on /proc/sys/fs/binfmt_misc type binfmt_misc (rw)
```

## Sự khác biệt giữa `/dev/random` và `/dev/urandom`?  
`/dev/random` là một tệp chứa trình tạo số ngẫu nhiên, trong đó entropy (độ ngẫu nhiên) được xác định từ nhiễu môi trường. `/dev/random` sẽ chặn / đợi cho đến khi đủ entropy được thu thập từ môi trường. 
 
`/dev/urandom` giống như `/dev/random`, nhưng khác ở chỗ nó cho phép lặp lại (ngưỡng entropy thấp hơn), do đó sẽ không bị chặn.

## Các hệ thống tệp khác
```bash
$ cat /proc/sys/kernel/random/entropy_avail
$ hexdump /dev/random
$ hexdump /dev/urandom

$ cat /proc/meminfo
$ cat /proc/cpuinfo
$ cat /proc/cpuinfo | grep bogomips

$ cat /proc/meminfo | grep Swap

$ cd /proc/self
$ echo $$; cd /proc/12345; cat maps
```

## Gắn kết hệ thống tệp

Giả sử bạn có một hệ thống tệp được kết nối trên `/dev/cdrom` mà bạn muốn đọc từ đó. Bạn phải gắn kết nó vào một thư mục trước khi bạn có thể thực hiện bất kỳ thao tác nào.
```bash
$ sudo mount /dev/cdrom /media/cdrom
$ mount
$ mount | grep proc
```

## Làm cách nào để gắn kết một ảnh đĩa?
Giả sử bạn đã tải xuống một ảnh đĩa Linux có thể khởi động:
```bash
wget http://cosmos.cites.illinois.edu/pub/archlinux/iso/2015.04.01/archlinux-2015.04.01-dual.iso
```
Trước khi ghi hệ thống tệp vào đĩa CD, bạn có thể gắn kết tệp dưới dạng hệ thống tệp và khám phá nội dung của nó. Lưu ý, `mount` yêu cầu quyền truy cập root, vì vậy hãy chạy nó bằng `sudo`:
```bash
$ mkdir arch
$ sudo mount -o loop archlinux-2015.04.01-dual.iso ./arch
$ cd arch
```
Trước lệnh `mount`, thư mục `arch` là mới và rõ ràng là trống. Sau khi gắn kết, nội dung của `arch/` sẽ được rút ra từ các tệp và thư mục được lưu trữ trong hệ thống tệp được lưu trữ bên trong tệp `archlinux-2014.11.01-dual.iso`.
Tùy chọn `loop` là bắt buộc vì chúng tôi muốn gắn kết một tệp thông thường chứ không phải là thiết bị khối chẳng hạn như đĩa vật lý. 

Tùy chọn vòng lặp bao bọc tệp gốc dưới dạng thiết bị khối - trong ví dụ này, chúng ta sẽ tìm hiểu bên dưới rằng hệ thống tệp được cung cấp dưới `/dev/loop0`: Chúng ta có thể kiểm tra loại hệ thống tệp và các tùy chọn gắn kết bằng cách chạy lệnh `mount` mà không có bất kỳ tham số nào. Chúng tôi sẽ chuyển đầu ra vào `grep` để chúng tôi chỉ xem các dòng đầu ra có liên quan chứa 'arch':
```bash
$ mount | grep arch
/home/demo/archlinux-2014.11.01-dual.iso on /home/demo/arch type iso9660 (rw,loop=/dev/loop0)
```
Hệ thống tệp `iso9660` là hệ thống tệp chỉ đọc, ban đầu được thiết kế cho phương tiện lưu trữ quang học (tức là CD-ROM). Nỗ lực thay đổi nội dung của hệ thống tệp sẽ thất bại:
```bash
$ touch arch/nocando
touch: cannot touch `/home/demo/arch/nocando': Read-only file system
```

[Chuyển đến Hệ thống tệp: Phần 6](https://github.com/angrave/SystemProgramming/wiki/File-System,-Part-6:-Memory-mapped-files-and-Shared-memory)


