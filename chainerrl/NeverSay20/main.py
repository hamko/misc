import chainer
import chainer.functions as F
import chainer.links as L
import chainerrl
import numpy as np
import random

#ゲームボード
class Board():
    def reset(self):
        self.n = np.array([0], dtype = np.float32);
        return self.n.copy()

    # return: obs(np.array), reward(double), done(bool), other info
    def step(self, act):
        self.n[0] += act + 1
        done = self.n[0] >= 20
        reward = 0
        if self.n[0] >= 20:
            reward = -1

        return self.n.copy(), reward, done, -1

    def actRandom(self):
        return random.randrange(3)

    def show(self):
        print("------------");
        print(self.n)

#explorer用のランダム関数オブジェクト
class RandomActor:
    def __init__(self, board):
        self.board = board
        self.random_count = 0
    def random_action_func(self):
        self.random_count += 1
        return self.board.actRandom()

#Q関数
class QFunction(chainer.Chain):
    def __init__(self, obs_size, n_actions, n_hidden_channels=81):
        super().__init__(
            l0=L.Linear(obs_size, n_hidden_channels),
            l1=L.Linear(n_hidden_channels, n_hidden_channels),
            l2=L.Linear(n_hidden_channels, n_actions))
    def __call__(self, x, test=False):
        #-1を扱うのでleaky_reluとした
        h = F.leaky_relu(self.l0(x))
        h = F.leaky_relu(self.l1(h))
        return chainerrl.action_value.DiscreteActionValue(self.l2(h))

# ボードの準備
b = Board()
# explorer用のランダム関数オブジェクトの準備
ra = RandomActor(b)
# 環境と行動の次元数
obs_size = 1
n_actions = 3
# Q-functionとオプティマイザーのセットアップ
q_func = QFunction(obs_size, n_actions)
optimizer = chainer.optimizers.Adam(eps=1e-2)
optimizer.setup(q_func)
# 報酬の割引率
gamma = 0.95
# Epsilon-greedyを使ってたまに冒険。50000ステップでend_epsilonとなる
explorer = chainerrl.explorers.LinearDecayEpsilonGreedy(
    start_epsilon=1.0, end_epsilon=0.3, decay_steps=50000, random_action_func=ra.random_action_func)
# Experience ReplayというDQNで用いる学習手法で使うバッファ
replay_buffer = chainerrl.replay_buffer.ReplayBuffer(capacity=10 ** 6)
# Agentの生成（replay_buffer等を共有する2つ）
agent_p1 = chainerrl.agents.DoubleDQN(
    q_func, optimizer, replay_buffer, gamma, explorer,
    replay_start_size=500, update_interval=1,
    target_update_interval=100)
agent_p2 = chainerrl.agents.DoubleDQN(
    q_func, optimizer, replay_buffer, gamma, explorer,
    replay_start_size=500, update_interval=1,
    target_update_interval=100)

#学習ゲーム回数
n_episodes = 2000
#カウンタの宣言
lose = 0
win = 0
#エピソードの繰り返し実行
for i in range(1, n_episodes + 1):
    agents = [agent_p1, agent_p2]
    turn = np.random.choice([0, 1])

    obs = b.reset()
    reward = 0
    last_state = 0;
    while 1:
#        pobs = obs
#        print(pobs)
        action = agents[turn].act_and_train(obs, reward) # ここでいうrewardというのは前回の行動によるrewardということ
        obs, reward, done, _ = b.step(action)
#        print(pobs, action, obs, done, reward);
        #配置の結果、終了時には報酬とカウンタに値をセットして学習
        if done:
            #エピソードを終了して学習
            agents[turn].stop_episode_and_train(b.n.copy(), reward, True)
            break
        else:
            #ターンを切り替え
            turn = 1 if turn == 0 else 0
            last_state = obs

    #コンソールに進捗表示
    if i % 100 == 0:
        print("episode:", i, " / rnd:", ra.random_count, " / statistics:", agent_p1.get_statistics(), " / epsilon:", agent_p1.explorer.epsilon)
        #カウンタの初期化
        lose = 0
        win = 0
        ra.random_count = 0
    if i % 10000 == 0:
        # 10000エピソードごとにモデルを保存
        agent_p1.save("result_" + str(i))

print("Training finished.")

#人間のプレーヤー
class HumanPlayer:
    def act(self, board):
        valid = False
        while not valid:
            try:
                act = input("Please enter 1-3: ")
                act = int(act)
                if act >= 1 and act <= 3:
                    valid = True
                    return act-1
                else:
                    print("Invalid move")
            except Exception as e:
                print(act +  " is invalid")

#検証
human_player = HumanPlayer()
for i in range(10):
    b.reset()
    dqn_first = np.random.choice([True, False])
    while 1:
        #DQN
        b.show()
        action = agent_p1.act(b.n.copy())
        obs, reward, done, _ = b.step(action)
        if done:
            if reward > 0:
                print("DQN Win")
            else:
                print("DQN Lose")
            agent_p1.stop_episode()
            break;

        #人間
        b.show()
        action = human_player.act(b.n.copy())
        obs, reward, done, _ = b.step(action)
        if done:
            if reward > 0:
                print("Human Win")
            else:
                print("Human Lose")
            agent_p1.stop_episode()
            break;

print("Test finished.")
