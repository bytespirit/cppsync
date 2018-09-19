.PHONY: build test

build:
	bazel build //:sync

test:
	bazel test //test:*

