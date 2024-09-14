title = 'Lập trình hệ thống'
filename = 'system-programming'
outputname=''system-programming'
revision = $(shell git describe --always --tags)
date = $(shell date +'%Y.%m.%d-%H:%M')
all: revision epub

revision:
	@echo '---' >> meta.markdown
	@echo 'title: "Lập trình hệ thống"' >> meta.markdown
	@echo 'author: UniBook  <system-programming[at]unibook.com>' >> meta.markdown
	@echo 'subtitle: |' >> meta.markdown
	@echo 'Nội dung trong tệp PDF này có thể đã lỗi thời, vui lòng kiểm tra trang web của chúng tôi <https://uniqbook.com/system-programming> hoặc kho lưu trữ GitHub <https://github.com/unibook/system-programming> để cập nhật sách mới nhất. Cập nhật lần cuối: ${date}' >> meta.markdown
	@echo 'rights: © Unibook, CC BY-NC-ND 4.0.' >> meta.markdown
	@echo 'ibooks:' >> meta.markdown
	@echo '  - version: ${revision}' >> meta.markdown
	@echo '---' >> meta.markdown

epub: markdown
	@echo "Compiling PDF file..."
	pandoc -f markdown+smart --toc -t epub -o $(filename).epub \
		--highlight-style haddock \
		--epub-cover-image ../../assets/cover-2nd-vi.png \
		--title-prefix $(title) \
		meta.markdown \
		00-preface.md.markdown \
		01-1-introduction.md.markdown \
		01-2-text-input-and-output.md.markdown \
		01-3-common-gotchas.md.markdown \ 
		01-4-string-and-struct.md.markdown \
		01-5-debugging.md.markdown \
		01-6-review-question.md.markdown \
		02-1-kernel-shells-terminals.md.markdown \
		02-2-process-introduction.md.markdown \
		02-3-porking-introduction.md.markdown \
		02-4-fork-exec-wait.md.markdown \
		02-5-wait-macros-using-signals.md.markdown \
		02-6-review-question.md.markdown \
		03-1-memory-introduction.md.markdown \
		03-2-memory-allocator.md.markdown \
		03-3-smashing-example.md.markdown \
		03-4-review-questions.md.markdown \
		04-1-pthread-introduction.md.markdown \
		04-3-parallel-problems.md.markdown \
		04-4-reviews-questions.md.markdown \
		appendix1.md.markdown \
		
	@echo "Done."
	rm -f *.md *.markdown

markdown:
	@echo "Copy markdown files..."
	cp -r ../../book/vi-vn/* .
	@echo "Aggregating markdown files..."
	python3 filter.py

clean:
	rm -rf *.md *.markdown

.PHONY: markdown epub clean