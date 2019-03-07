
{
  "targets": [
    {
      "target_name": "value_array",
      "include_dirs": [
        "../../../include"
      ],
      
      "libraries": [],
      
      "sources": ["binding.cpp"],
      
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
