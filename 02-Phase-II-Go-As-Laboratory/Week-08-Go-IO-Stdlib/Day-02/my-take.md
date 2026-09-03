So Go, has two ways of decoding json files, `json.Unmarshal` and `json.Decoder`. Json Unmarshal is used when you have the data available in memory, while json Decoder is used whe you want to read the data from a stream, such as a file or an HTTP response.

We also need to ensure and validiate any incoming data, so we can use the `json.Valid` function to check if the data is valid json before decoding it.
