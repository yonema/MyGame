#pragma once



/// <summary>
/// �K�E�V�A���u���[�N���X�B
/// </summary>
class CGaussianBlur {
public:		//�����o�֐�
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="originalTexture">�K�E�V�A���u���[��������I���W�i���e�N�X�`���B</param>
	void Init(Texture* originalTexture);

	/// <summary>
	/// �K�E�V�A���u���[��GPU��Ŏ��s�B
	/// </summary>
	/// <remarks>
	/// �{�֐��̌Ăяo���́ADirectX12�𗘗p�����`��R�}���h�������ɌĂяo���K�v������܂��B
	/// </remarks>
	/// <param name="rc">�����_�����O�^�[�Q�b�g</param>
	/// <param name="blurPower">�u���[�̋����B�l���傫���قǃ{�P�������Ȃ�B</param>
	void ExecuteOnGPU(RenderContext& rc, float blurPower);

	/// <summary>
	/// �{�P�e�N�X�`�����擾�B
	/// </summary>
	/// <returns></returns>
	Texture& GetBokeTexture()
	{
		return m_yBlurRenderTarget.GetRenderTargetTexture();
	}

private:	//private�ȃ����o�֐�
	/// <summary>
	/// �����_�����O�^�[�Q�b�g���������B
	/// </summary>
	void InitRenderTargets();

	/// <summary>
	/// �X�v���C�g���������B
	/// </summary>
	void InitSprites();

	/// <summary>
	/// �d�݃e�[�u�����X�V����B
	/// </summary>
	void UpdateWeightsTable(float blurPower);

private:	//�f�[�^�����o
	enum { NUM_WEIGHTS = 8 };				//�d�݂̐��B
	float m_weights[NUM_WEIGHTS];			//�d�݃e�[�u���B
	Texture* m_originalTexture = nullptr;	//�I���W�i���e�N�X�`���B
	RenderTarget m_xBlurRenderTarget;		//���{�P�摜��`�悷�郌���_�����O�^�[�Q�b�g�B
	RenderTarget m_yBlurRenderTarget;		//�c�{�P�摜��`�悷�郌���_�����O�^�[�Q�b�g�B
	Sprite m_xBlurSprite;					//���{�P�摜��`�悷�邽�߂̃X�v���C�g�B
	Sprite m_yBlurSprite;					//�c�{�P�摜��`�悷�邽�߂̃X�v���C�g�B
};
