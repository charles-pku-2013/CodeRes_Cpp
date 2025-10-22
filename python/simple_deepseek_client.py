from openai import OpenAI

def test():
    try:
        # client = DeepSeekSimpleClient(
                        # api_key="...",
                        # base_url="...")

        # prompt = "你是谁？"
        # response = client.chat(prompt)
        # print(response)
        # print(type(response))  # dict

        client = OpenAI(api_key= '...', 
                        base_url= '...')

        prompt = "who are you?"

        completion = client.chat.completions.create(
            model='deepseek-r1',
            messages=[
                {'role': 'user', 'content': prompt}
            ],
            # stream=True,
            extra_body={"thinking": {"type": "disabled"}},
        )

        # 一点点输出
        # for chunk in completion:
            # delta = chunk.choices[0].delta
            # print(delta.content, end='', flush=True)

        # print(completion)
        # print(type(completion))
        # print(type(completion.choices))
        # print(type(completion.choices[0]))
        print(completion.choices[0].message.content)
        # print(completion.choices[0].message.reasoning_content)

    except Exception as e:
        logging.error(f"Error {e}")

def test2():
    client = OpenAI(api_key= '...', 
                    base_url= '...')

    prompt = (
        "Translate the following text into Japanese. "
        "Only respond with the translated text, nothing else.\n\n"
        "Text: 今天天气不错！"
    )

    response = client.chat.completions.create(
        model="deepseek-r1", # Or other available DeepSeek models
        messages=[{"role": "user", "content": prompt}]
    )
    translated_text = response.choices[0].message.content

    print(translated_text)

if __name__ == "__main__":
    # test()
    test2()
