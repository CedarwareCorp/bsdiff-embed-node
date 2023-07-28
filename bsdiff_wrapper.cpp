extern "C" {
#include "bsdiff.h"
}
#include <napi.h>
#include <cstdlib>
#include <vector>

int buffer_write(struct bsdiff_stream* stream, const void* buffer, int size)
{
    std::vector<uint8_t>* out_buffer = (std::vector<uint8_t>*) stream->opaque;

    uint8_t* buff = (uint8_t*) buffer;

    out_buffer->insert(out_buffer->end(), &buff[0], &buff[size]);

	return 0;
}

Napi::Value Bsdiff(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsBuffer() || !info[1].IsBuffer()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    Napi::Buffer<uint8_t> old_buffer = info[0].As<Napi::Buffer<uint8_t>>();
    Napi::Buffer<uint8_t> new_buffer = info[1].As<Napi::Buffer<uint8_t>>();

    std::vector<uint8_t> out_buffer;
    struct bsdiff_stream stream = {
        .opaque = &out_buffer,
        .malloc = malloc,
        .free = free,
        .write = buffer_write,
    };

    int result = bsdiff(old_buffer.Data(), old_buffer.Length(), new_buffer.Data(), new_buffer.Length(), &stream);

    return Napi::Buffer<uint8_t>::Copy(env, out_buffer.data(), out_buffer.size());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "bsdiff"), Napi::Function::New(env, Bsdiff));
  return exports;
}

NODE_API_MODULE(addon, Init)
