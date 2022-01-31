# cpp-game

A collection of simple games and experiments written in C++, ncurses, graphviz and built by bazel.

## Projects

### life

```bash
bazel run //life:main -- -c 100 -r 50 -g 10000 -d 100
```

###  graph

```bash
bazel run //graph/application:fully-connected -- 10           # complete graph (dot output)
bazel run //graph/application:random-connected --  10 100     # randomly connected (dot output)
bazel run //graph/application:matrix -- 10 100                # prints adjacency matrix representation (custom output)
bazel run //graph/application:structure --  10 100            # prints adj-list structure (custom output)
bazel run //graph/application:bfs-search --  10 100           # Does a BFS from vertex 1 -> n
```

### util

```
bazel run //util:cppversion
```

## TODO

Unified CLI so for any program, you can choose output format (matrix, structure, dot).



-- Installing: /usr/local/lib/libyaml-cpp.a
-- Up-to-date: /usr/local/include
-- Installing: /usr/local/include/yaml-cpp
-- Installing: /usr/local/include/yaml-cpp/stlemitter.h
-- Installing: /usr/local/include/yaml-cpp/anchor.h
-- Installing: /usr/local/include/yaml-cpp/noexcept.h
-- Installing: /usr/local/include/yaml-cpp/node
-- Installing: /usr/local/include/yaml-cpp/node/parse.h
-- Installing: /usr/local/include/yaml-cpp/node/node.h
-- Installing: /usr/local/include/yaml-cpp/node/ptr.h
-- Installing: /usr/local/include/yaml-cpp/node/convert.h
-- Installing: /usr/local/include/yaml-cpp/node/impl.h
-- Installing: /usr/local/include/yaml-cpp/node/emit.h
-- Installing: /usr/local/include/yaml-cpp/node/type.h
-- Installing: /usr/local/include/yaml-cpp/node/detail
-- Installing: /usr/local/include/yaml-cpp/node/detail/memory.h
-- Installing: /usr/local/include/yaml-cpp/node/detail/iterator_fwd.h
-- Installing: /usr/local/include/yaml-cpp/node/detail/node_iterator.h
-- Installing: /usr/local/include/yaml-cpp/node/detail/node.h
-- Installing: /usr/local/include/yaml-cpp/node/detail/node_ref.h
-- Installing: /usr/local/include/yaml-cpp/node/detail/node_data.h
-- Installing: /usr/local/include/yaml-cpp/node/detail/impl.h
-- Installing: /usr/local/include/yaml-cpp/node/detail/iterator.h
-- Installing: /usr/local/include/yaml-cpp/node/iterator.h
-- Installing: /usr/local/include/yaml-cpp/emitter.h
-- Installing: /usr/local/include/yaml-cpp/contrib
-- Installing: /usr/local/include/yaml-cpp/contrib/anchordict.h
-- Installing: /usr/local/include/yaml-cpp/contrib/graphbuilder.h
-- Installing: /usr/local/include/yaml-cpp/emitfromevents.h
-- Installing: /usr/local/include/yaml-cpp/binary.h
-- Installing: /usr/local/include/yaml-cpp/parser.h
-- Installing: /usr/local/include/yaml-cpp/null.h
-- Installing: /usr/local/include/yaml-cpp/traits.h
-- Installing: /usr/local/include/yaml-cpp/emitterstyle.h
-- Installing: /usr/local/include/yaml-cpp/depthguard.h
-- Installing: /usr/local/include/yaml-cpp/eventhandler.h
-- Installing: /usr/local/include/yaml-cpp/dll.h
-- Installing: /usr/local/include/yaml-cpp/exceptions.h
-- Installing: /usr/local/include/yaml-cpp/emittermanip.h
-- Installing: /usr/local/include/yaml-cpp/ostream_wrapper.h
-- Installing: /usr/local/include/yaml-cpp/yaml.h
-- Installing: /usr/local/include/yaml-cpp/mark.h
-- Installing: /usr/local/include/yaml-cpp/emitterdef.h
-- Installing: /usr/local/share/cmake/yaml-cpp/yaml-cpp-targets.cmake
-- Installing: /usr/local/share/cmake/yaml-cpp/yaml-cpp-targets-noconfig.cmake
-- Installing: /usr/local/share/cmake/yaml-cpp/yaml-cpp-config.cmake
-- Installing: /usr/local/share/cmake/yaml-cpp/yaml-cpp-config-version.cmake
-- Installing: /usr/local/share/pkgconfig/yaml-cpp.pc
-- Up-to-date: /usr/local/include
-- Installing: /usr/local/include/gmock
-- Installing: /usr/local/include/gmock/gmock-matchers.h
-- Installing: /usr/local/include/gmock/gmock-function-mocker.h
-- Installing: /usr/local/include/gmock/internal
-- Installing: /usr/local/include/gmock/internal/gmock-pp.h
-- Installing: /usr/local/include/gmock/internal/gmock-internal-utils.h
-- Installing: /usr/local/include/gmock/internal/gmock-port.h
-- Installing: /usr/local/include/gmock/internal/custom
-- Installing: /usr/local/include/gmock/internal/custom/gmock-port.h
-- Installing: /usr/local/include/gmock/internal/custom/gmock-matchers.h
-- Installing: /usr/local/include/gmock/internal/custom/gmock-generated-actions.h
-- Installing: /usr/local/include/gmock/internal/custom/README.md
-- Installing: /usr/local/include/gmock/internal/custom/gmock-generated-actions.h.pump
-- Installing: /usr/local/include/gmock/gmock-generated-actions.h
-- Installing: /usr/local/include/gmock/gmock-generated-function-mockers.h
-- Installing: /usr/local/include/gmock/gmock-more-actions.h
-- Installing: /usr/local/include/gmock/gmock-more-matchers.h
-- Installing: /usr/local/include/gmock/gmock-nice-strict.h
-- Installing: /usr/local/include/gmock/gmock-generated-function-mockers.h.pump
-- Installing: /usr/local/include/gmock/gmock.h
-- Installing: /usr/local/include/gmock/gmock-cardinalities.h
-- Installing: /usr/local/include/gmock/gmock-spec-builders.h
-- Installing: /usr/local/include/gmock/gmock-actions.h
-- Installing: /usr/local/include/gmock/gmock-generated-actions.h.pump
-- Installing: /usr/local/include/gmock/gmock-generated-matchers.h
-- Installing: /usr/local/include/gmock/gmock-generated-matchers.h.pump
-- Installing: /usr/local/lib/libgmock.a
-- Installing: /usr/local/lib/libgmock_main.a
-- Installing: /usr/local/lib/pkgconfig/gmock.pc
-- Installing: /usr/local/lib/pkgconfig/gmock_main.pc
-- Installing: /usr/local/lib/cmake/GTest/GTestTargets.cmake
-- Installing: /usr/local/lib/cmake/GTest/GTestTargets-noconfig.cmake
-- Installing: /usr/local/lib/cmake/GTest/GTestConfigVersion.cmake
-- Installing: /usr/local/lib/cmake/GTest/GTestConfig.cmake
-- Up-to-date: /usr/local/include
-- Installing: /usr/local/include/gtest
-- Installing: /usr/local/include/gtest/gtest-param-test.h
-- Installing: /usr/local/include/gtest/internal
-- Installing: /usr/local/include/gtest/internal/gtest-port-arch.h
-- Installing: /usr/local/include/gtest/internal/gtest-string.h
-- Installing: /usr/local/include/gtest/internal/gtest-death-test-internal.h
-- Installing: /usr/local/include/gtest/internal/gtest-type-util.h.pump
-- Installing: /usr/local/include/gtest/internal/gtest-type-util.h
-- Installing: /usr/local/include/gtest/internal/gtest-port.h
-- Installing: /usr/local/include/gtest/internal/gtest-internal.h
-- Installing: /usr/local/include/gtest/internal/gtest-param-util.h
-- Installing: /usr/local/include/gtest/internal/gtest-filepath.h
-- Installing: /usr/local/include/gtest/internal/custom
-- Installing: /usr/local/include/gtest/internal/custom/README.md
-- Installing: /usr/local/include/gtest/internal/custom/gtest.h
-- Installing: /usr/local/include/gtest/internal/custom/gtest-port.h
-- Installing: /usr/local/include/gtest/internal/custom/gtest-printers.h
-- Installing: /usr/local/include/gtest/gtest-matchers.h
-- Installing: /usr/local/include/gtest/gtest-death-test.h
-- Installing: /usr/local/include/gtest/gtest-spi.h
-- Installing: /usr/local/include/gtest/gtest.h
-- Installing: /usr/local/include/gtest/gtest-test-part.h
-- Installing: /usr/local/include/gtest/gtest-typed-test.h
-- Installing: /usr/local/include/gtest/gtest_prod.h
-- Installing: /usr/local/include/gtest/gtest_pred_impl.h
-- Installing: /usr/local/include/gtest/gtest-message.h
-- Installing: /usr/local/include/gtest/gtest-printers.h
-- Installing: /usr/local/lib/libgtest.a
-- Installing: /usr/local/lib/libgtest_main.a
-- Installing: /usr/local/lib/pkgconfig/gtest.pc
-- Installing: /usr/local/lib/pkgconfig/gtest_main.pc