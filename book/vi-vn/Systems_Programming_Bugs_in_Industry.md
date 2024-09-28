# Hãy cẩn thận!

Dưới đây là tất cả các lỗi nổi tiếng đã làm sập hệ thống. Nếu bạn muốn thử nghiệm với bất kỳ mã khởi động nào mà chúng cung cấp, hãy thận trọng vì bạn có thể mất các tệp quan trọng hoặc thậm chí khả năng sử dụng hệ thống của mình.

Bạn có thể không hiểu nhiều về điều này khi bắt đầu khóa học. Khi chúng ta tìm hiểu, ngày càng nhiều lỗi trong số này sẽ trở nên rõ ràng. Lập trình hệ thống không nhất thiết phải chỉ bằng C!

## Lỗi bộ nhớ/C cơ bản
[Lỗ hổng trong IOHIDeous, Mac OS X](https://siguza.github.io/IOHIDeous/)
[Heartbleed](https://xkcd.com/1354/)
[Lỗi bộ nhớ PHP](http://www.inulledmyself.com/2015/05/exploiting-memory-corruption-bugs-in.html) -- Vui lòng không sử dụng PHP
["Cloudbleed"](https://bugs.chromium.org/p/project-zero/issues/detail?id=1139)

## Các điều kiện tranh đua nổi tiếng
[Kiếm tiền trên DigitalOcean](http://josipfranjkovic.blogspot.com/2015/04/race-conditions-on-facebook.html),
[Điều kiện tranh đua tuyệt vời](http://mailinator.blogspot.com/2009/06/beautiful-race-condition.html),
[Điều kiện tranh đua Bitcoin](https://www.josipfranjkovic.com/blog/race-conditions-on-web),
[Dirty COW, Lỗi yêu thích của tôi](https://github.com/dirtycow/dirtycow.github.io/wiki/VulnerabilityDetails),
[Siêu phân luồng](http://gallium.inria.fr/blog/intel-skylake-bug/),
[Điều kiện tranh đua trong Kernel](http://seclists.org/oss-sec/2013/q1/326),
Không phải là lỗi nhưng vẫn rất tuyệt -- [Tạo số ngẫu nhiên với điều kiện tranh đua](https://github.com/dasmithii/RCRand)

## Bộ nhớ
[Phân bổ bộ nhớ trong Python](https://lukasa.co.uk/2016/12/Debugging_Your_Operating_System/?hn=1),
[Rò rỉ bộ nhớ](https://www.joyent.com/blog/walmart-node-js-memory-leak),
[`kfree` hai lần để trở thành root](http://seclists.org/oss-sec/2017/q1/471)

## Tiến trình
[Sử dụng waitid để thoát khỏi Docker](https://www.twistlock.com/2017/12/27/escaping-docker-container-using-waitid-cve-2017-5123/),
[Lỗi `fork` nổi tiếng](http://rachelbythebay.com/w/2014/08/19/fork/)

## Hệ thống tệp
[Hệ thống tệp của Apple](https://cxsecurity.com/issue/WLB-2015100149?),
[Hệ thống tệp của Ubuntu](http://seclists.org/oss-sec/2015/q2/717)

## I/O hướng sự kiện
[`epoll` bị hỏng](https://idea.popcount.org/2017-02-20-epoll-is-fundamentally-broken-12/)

## Mạng
[Tràn bộ nhớ đệm gethostbyname](http://www.openwall.com/lists/oss-security/2015/01/27/9),
[Đánh lừa phần mềm diệt virus](https://bugs.chromium.org/p/project-zero/issues/detail?id=820),
[Phá vỡ SSL](https://guidovranken.wordpress.com/2016/02/27/openssl-cve-2016-0799-heap-corruption-via-bio_printf/)

## Lập lịch
[Hàng thập kỷ lãng phí lõi](http://www.ece.ubc.ca/~sasha/papers/eurosys16-final29.pdf)

## Tín hiệu
[Nâng cấp đặc quyền với `kill(-1, SIGKILL)`](http://www.openwall.com/lists/oss-security/2017/10/12/1)

