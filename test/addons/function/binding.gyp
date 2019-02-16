
{
  "targets": [
    {
      "target_name": "function",
      "include_dirs": [
        "../../../include"
      ],
      
      "libraries": [],
      
      "sources": ["binding.cpp","_embind.cpp" ],
      
      "conditions": [
          [
            "OS==\"linux\"", 
            {"cflags": 
              ["-O2", "-std=c++11"],
              "cflags_cc": ["-O2", "-std=c++11"]
            }
          ]
      ]
    }
  ]
}
