{
  "targets": [
    {
      "target_name": "py2node",
      'libraries': [
        '<!@(pkg-config python --libs)'
      ],
      'include_dirs': [
        '<!@(pkg-config python --cflags-only-I | sed s/-I//g)',
      ],
      "sources": [
        "src/main.cc",
        "src/pyobject2js.cc",
        "src/strings.cc"
      ]
    }
  ],
}
