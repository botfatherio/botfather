.PHONY: build
build:
	flatpak-builder --force-clean build-dir io.botfather.Nightly.yaml
	@echo
	@echo "TODO:"
	@echo "- Add the new version to the release appdata.xml file"
	@echo "- Archive and upload the contents of build-dir/files/bin"
	@echo "- Update url, filesize and hash in the release Botfather.yaml file"
	@echo

.PHONY: clean
clean:
	rm -r build-dir
