.PHONY: format
format:
	find . -regex '\.\/\(botfather\|webhelper\)\/.*\.\(cpp\|hpp\)' -exec clang-format -i {} \;

