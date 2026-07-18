.PHONY: format
format:
	find . -regex '\.\/\(botfather\|webhelper\)\/.*\.\(cpp\|hpp\)' -exec clang-format -i {} \;

.PHONY: flatpak
flatpak:
	flatpak-builder --force-clean build-dir flatpak/io.botfather.Nightly.yaml
